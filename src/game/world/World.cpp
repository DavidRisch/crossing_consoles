#include "World.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>

#include "items/DeserializeItemUtils.h"

using namespace game;
using namespace game::common;
using namespace game::world;

World::World(coordinate_size_t size)
    : size(std::move(size))
    , spawner(this)
    , item_generator(this) {
}

World::World()
    : World(common::coordinate_size_t(0, 0)) {
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

void World::AddItem(const Position& position, const std::shared_ptr<IItem>& item) {
  assert(item);
  items.insert({position, item});
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

bool World::IsBlockedForItem(const Position& position) {
  if (IsBlocked(position) || items.find(position) != items.end()) {
    return true;
  }
  return false;
}

void World::UpdateWalls(const World& server_world) {
  size = server_world.size;
  walls = server_world.walls;

  updated = true;
}

void World::UpdateWithoutWalls(const World& server_world) {
  assert(size == server_world.size);

  projectiles = server_world.projectiles;
  items = server_world.items;
  colored_fields = server_world.colored_fields;

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

  ISerializable::SerializeMap(output_vector, walls, false);

  SerializeUpdate(output_vector);
}

void World::SerializeUpdate(std::vector<uint8_t>& output_vector) const {
  ISerializable::SerializeList(output_vector, players);

  // Serialize items
  output_vector.push_back(items.size());
  for (const auto& pair : items) {
    pair.first.Serialize(output_vector);
    networking::SerializationUtils::SerializeObject(pair.second->GetItemType(), output_vector);
    pair.second->Serialize(output_vector);
  }

  ISerializable::SerializeList(output_vector, projectiles);
  ISerializable::SerializeMap(output_vector, colored_fields, false);
}

World World::Deserialize(std::vector<uint8_t>::iterator& input_iterator) {
  auto size = coordinate_size_t::Deserialize(input_iterator);
  auto world = World(size);

  auto wall_count = ISerializable::DeserializeContainerLength(input_iterator);
  for (size_t i = 0; i < wall_count; ++i) {
    auto wall = Wall::Deserialize(input_iterator);
    world.AddWall(wall.position, wall.type);
  }

  DeserializeUpdate(input_iterator, world);

  return world;
}

World World::DeserializeUpdate(std::vector<uint8_t>::iterator& input_iterator, World& world) {
  auto player_count = ISerializable::DeserializeContainerLength(input_iterator);
  for (size_t i = 0; i < player_count; ++i) {
    auto player = std::make_shared<Player>(Player::Deserialize(input_iterator));
    world.AddPlayer(player);
  }

  auto item_count = game::world::DeserializeItemUtils::DeserializeItemNumber(input_iterator);
  for (size_t i = 0; i < item_count; ++i) {
    auto position = Position::Deserialize(input_iterator);
    auto item_type = networking::SerializationUtils::DeserializeObject<ItemType>(input_iterator);
    auto new_item = game::world::DeserializeItemUtils::DeserializeItem(item_type, input_iterator);
    world.AddItem(position, new_item);
  }

  auto projectile_count = ISerializable::DeserializeContainerLength(input_iterator);
  for (size_t i = 0; i < projectile_count; ++i) {
    auto projectile = std::make_shared<Projectile>(Projectile::Deserialize(input_iterator));
    world.AddProjectile(projectile);
  }

  auto colored_field_count = ISerializable::DeserializeContainerLength(input_iterator);
  for (size_t i = 0; i < colored_field_count; ++i) {
    auto colored_field = ColoredField::Deserialize(input_iterator);
    world.AddColoredField(colored_field);
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
  player.IncreaseHealth(game::world::Player::max_health);
  player.RemoveWeapon();
  player.score = 0;
  player.position = spawner.GenerateSpawnPosition();
}

ItemGenerator& World::GetItemGenerator() {
  return item_generator;
}

void World::AddColoredField(const ColoredField& colored_field) {
  colored_fields.insert_or_assign(colored_field.GetPosition(), colored_field);
}
