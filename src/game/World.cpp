#include "World.h"

#include <algorithm>

World::World(int width, int height)
    : size(Position(width, height)) {
}

void World::AddPlayer(Player* player) {
  players.emplace_back(player);
  updated = true;
}

void World::AddWall(const Position& position) {
  walls.emplace_back(new Wall(position));
  updated = true;
}

bool World::IsWall(const Position& position) {
  for (auto const& i_wall : walls) {
    if (i_wall->position == position) {
      return true;
    }
  }
  return false;
}