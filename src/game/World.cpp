#include "World.h"

#include <algorithm>

World::World(coordinate_size_t size)
    : size(size) {
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

void World::Serialize(std::vector<uint8_t>& into) const {
  size.Serialize(into);

  ISerializable::SerializeList(into, walls);

  ISerializable::SerializeList(into, players);
}

World World::Deserialize(std::vector<uint8_t>::iterator& from) {
  auto size = coordinate_size_t::Deserialize(from);
  auto world = World(size);

  auto wall_count = ISerializable::DeserializeListLength(from);
  for (size_t i = 0; i < wall_count; ++i) {
    world.AddWall(Wall::Deserialize(from).position);
  }

  auto player_count = ISerializable::DeserializeListLength(from);
  for (size_t i = 0; i < player_count; ++i) {
    auto* player = new Player(Player::Deserialize(from));  // TODO: fix memory leak
    world.AddPlayer(player);
  }

  return world;
}
