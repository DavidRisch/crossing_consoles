#include "World.h"

#include <algorithm>
#include <cassert>
#include <utility>

using namespace game;
using namespace game::common;
using namespace game::world;

World::World(coordinate_size_t size)
    : size(std::move(size))
    , spawner(this) {
}

void World::AddPlayer(const std::shared_ptr<Player>& player) {
  assert(player != nullptr);
  players.push_back(player);
  updated = true;
}

void World::RemovePlayer(GameDefinition::player_id_t player_id) {
  auto it = std::find_if(players.begin(), players.end(), [&player_id](const std::shared_ptr<Player>& player) {
    return player->player_id == player_id;
  });
  assert(it != players.end());
  players.erase(it);
}

void World::AddWall(const Position& position, BlockType type) {
  if (position.IsGreaterOrEqual(Position(0, 0)) && position.IsLess(size)) {
    if (walls.find(position) != walls.end()) {
      walls.at(position) = Wall(position, type);
    } else {
      walls.emplace(position, Wall(position, type));
    }

    updated = true;
  }
}

void World::RemoveWall(const Position& position) {
  if (walls.find(position) != walls.end()) {
    walls.erase(position);
    updated = true;
  }
}

bool World::IsBlocked(const Position& position) {
  if (walls.find(position) != walls.end()) {
    return true;
  }

  if (std::any_of(players.begin(), players.end(),
                  [&position](const std::shared_ptr<Player>& player) { return player->position == position; })) {
    return true;
  }

  return false;
}

void World::Update(const World& server_world) {
  size = server_world.size;
  projectiles = server_world.projectiles;
  walls = server_world.walls;

  for (const auto& server_player : server_world.players) {
    auto player_id = server_player->player_id;
    auto client_player = GetPlayerById(player_id);
    if (client_player == nullptr) {
      // add new player
      players.push_back(server_player);
    } else {
      // modify existing player
      assert(client_player->player_id == server_player->player_id);
      *client_player = *server_player;
    }
  }

  // remove players no longer on the server
  auto removed_players_begin =
      std::remove_if(players.begin(), players.end(), [&server_world](const std::shared_ptr<Player>& player) {
        return server_world.GetPlayerById(player->player_id) == nullptr;
      });
  players.erase(removed_players_begin, players.end());

  updated = true;
}

std::shared_ptr<Player> World::GetPlayerById(int player_id) const {
  auto client_player_it =
      std::find_if(players.begin(), players.end(),
                   [&player_id](const std::shared_ptr<Player>& player) { return player->player_id == player_id; });
  if (client_player_it != players.end()) {
    return *client_player_it;
  } else {
    return std::shared_ptr<Player>();
  }
}

const Spawner& World::GetSpawner() const {
  return spawner;
}

void World::Serialize(std::vector<uint8_t>& output_vector) const {
  size.Serialize(output_vector);

  ISerializable::SerializeMap(output_vector, walls);

  ISerializable::SerializeList(output_vector, players);

  ISerializable::SerializeList(output_vector, projectiles);
}

World World::Deserialize(std::vector<uint8_t>::iterator& input_iterator) {
  auto size = coordinate_size_t::Deserialize(input_iterator);
  auto world = World(size);

  auto wall_count = ISerializable::DeserializeContainerLength(input_iterator);
  for (size_t i = 0; i < wall_count; ++i) {
    auto position = Position::Deserialize(input_iterator);
    auto wall = Wall::Deserialize(input_iterator);
    assert(position == wall.position);
    // TODO: this needs to be extended when Wall has any attributes in addition to position
    world.AddWall(position);
  }

  auto player_count = ISerializable::DeserializeContainerLength(input_iterator);
  for (size_t i = 0; i < player_count; ++i) {
    auto player = std::make_shared<Player>(Player::Deserialize(input_iterator));
    world.AddPlayer(player);
  }

  auto projectile_count = ISerializable::DeserializeContainerLength(input_iterator);
  for (size_t i = 0; i < projectile_count; ++i) {
    auto projectile = std::make_shared<Projectile>(Projectile::Deserialize(input_iterator));
    world.AddProjectile(projectile);
  }

  return world;
}

std::list<std::shared_ptr<Projectile>> World::GetProjectiles() {
  return projectiles;
}

void World::AddProjectile(const std::shared_ptr<Projectile>& projectile) {
  auto world_projectile_it = std::find_if(projectiles.begin(), projectiles.end(),
                                          [&projectile](const std::shared_ptr<Projectile>& world_projectile) {
                                            return world_projectile->GetPosition() == projectile->GetPosition();
                                          });

  if (world_projectile_it != projectiles.end()) {
    if (projectile->GetDirection() == (*world_projectile_it)->GetDirection()) {
      assert(projectile->GetShooterId() == (*world_projectile_it)->GetShooterId());
      // Do not add multiple projectiles of same player with identical position to world
      return;
    }
  }

  projectiles.push_back(projectile);
  updated = true;
}

void World::RemoveProjectiles(std::list<std::shared_ptr<Projectile>> list_to_remove) {
  for (auto& remove_projectile : list_to_remove) {
    auto iterator = std::remove_if(
        projectiles.begin(), projectiles.end(),
        [&remove_projectile](const std::shared_ptr<Projectile>& item) { return item == remove_projectile; });
    projectiles.erase(iterator, projectiles.end());
  }
}

std::optional<std::shared_ptr<Projectile>> World::GetProjectileFromPosition(common::Position position) {
  auto projectiles_it = std::find_if(
      projectiles.begin(), projectiles.end(),
      [&position](const std::shared_ptr<Projectile>& projectile) { return projectile->GetPosition() == position; });
  if (projectiles_it != projectiles.end()) {
    return *projectiles_it;
  }
  return std::optional<std::shared_ptr<Projectile>>();
}

void World::ResurrectPlayer(Player& player) {
  player.DecreaseHealth(-game::world::Player::max_health);
  player.score = 0;
  player.position = spawner.GenerateSpawnPosition();
}
