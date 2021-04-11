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
//  players.end() =
//      std::remove_if(players.begin(), players.end(), [&server_world](const std::shared_ptr<Player>& player) {
//        return server_world.GetPlayerById(player->player_id) == nullptr;
//      });

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

  return world;
}
