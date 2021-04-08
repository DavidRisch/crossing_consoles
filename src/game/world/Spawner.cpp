#include "Spawner.h"

#include "World.h"

using namespace game;
using namespace game::world;
using namespace game::common;

Spawner::Spawner(World* world)
    : world(world) {
}

Position Spawner::GenerateSpawnPosition() const {
  Position position(0, 0);
  while (world->IsBlockedForItem(position)) {
    position.x++;
    if (position.x >= world->size.x) {
      position.x = 0;
      position.y++;
      if (position.y >= world->size.y) {
        throw NoSpaceException();
      }
    }
  }

  return position;
}
