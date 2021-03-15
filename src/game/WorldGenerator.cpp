#include "WorldGenerator.h"

std::shared_ptr<World> WorldGenerator::GenerateWorld(const coordinate_size_t& world_size) {
  auto world = std::make_shared<World>(world_size);

  world->AddWall(Position(1, 1));
  world->AddWall(Position(2, 2));
  world->AddWall(Position(3, 3));
  world->AddWall(Position(4, 4));

  return world;
}
