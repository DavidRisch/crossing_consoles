#include "World.h"

World::World(int width, int height)
    : size(Position(width, height)) {
}

void World::AddWall(const Position& position) {
  walls.emplace_back(position);
  updated = true;
}