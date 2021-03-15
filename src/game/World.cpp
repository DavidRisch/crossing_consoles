#include "World.h"

#include <cassert>
#include <utility>

World::World(coordinate_size_t size)
    : size(std::move(size)) {
}

void World::AddPlayer(Player* player) {
  players.emplace_back(player);
  updated = true;
}

void World::AddWall(const Position& position) {
  if (position > Position(0, 0) && position < size) {
    walls.emplace_back(new Wall(position));
    updated = true;
  }
}

bool World::IsBlocked(const Position& position) {
  for (auto const& i_wall : walls) {
    if (i_wall->position == position) {
      return true;
    }
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

void World::Serialize(std::vector<uint8_t>& output_vector) const {
  size.Serialize(output_vector);

  ISerializable::SerializeList(output_vector, walls);

  ISerializable::SerializeList(output_vector, players);
}

World World::Deserialize(std::vector<uint8_t>::iterator& input_iterator) {
  auto size = coordinate_size_t::Deserialize(input_iterator);
  auto world = World(size);

  auto wall_count = ISerializable::DeserializeListLength(input_iterator);
  for (size_t i = 0; i < wall_count; ++i) {
    world.AddWall(Wall::Deserialize(input_iterator).position);
  }

  auto player_count = ISerializable::DeserializeListLength(input_iterator);
  for (size_t i = 0; i < player_count; ++i) {
    auto* player = new Player(Player::Deserialize(input_iterator));  // TODO: fix memory leak
    world.AddPlayer(player);
  }

  return world;
}
