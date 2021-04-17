#include "Spawner.h"

#include "World.h"

using namespace game;
using namespace game::world;
using namespace game::common;

std::random_device Spawner::random_device;

Spawner::Spawner(World* world)
    : world(world) {
}

Position Spawner::GenerateSpawnPosition() const {
  // generate a random position
  std::uniform_int_distribution<int> x_coordinate(0, world->size.x - 1);
  int random_x = x_coordinate(random_device);
  std::uniform_int_distribution<int> y_coordinate(0, world->size.y - 1);
  int random_y = y_coordinate(random_device);
  Position random_position(random_x, random_y);

  Position new_position = random_position;
  // move to next position if current is blocked
  while (world->IsBlockedForItem(new_position)) {
    new_position.x++;
    if (new_position.x >= world->size.x) {
      new_position.x = 0;
      new_position.y++;
      if (new_position.y >= world->size.y) {
        new_position.y = 0;
      }
    }
    if (new_position == random_position) {
      throw NoSpaceException();
    }
  }

  return new_position;
}
