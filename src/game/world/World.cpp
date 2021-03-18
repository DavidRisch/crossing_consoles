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

void World::AddWall(const Position& position) {
  if (position >= Position(0, 0) && position < size) {
    if (walls.find(position) != walls.end()) {
      walls.at(position) = Wall(position);
    } else {
      walls.emplace(position, position);
    }

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

  // TODO: more advanced setup require to handle multiple players
  assert(players.size() == 1);
  if (!server_world.players.empty()) {
    assert(server_world.players.size() == 1);
    players.front() = server_world.players.front();
  }

  updated = true;
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
    auto player = std::make_shared<Player>(Player::Deserialize(input_iterator));  // TODO: fix memory leak
    world.AddPlayer(player);
  }

  return world;
}
