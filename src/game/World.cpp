#include "World.h"

World::World(int width, int height) {
  size[0] = width;
  size[1] = height;
}

bool World::IsWall(Position position) {
  return false;
}
