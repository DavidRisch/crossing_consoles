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
