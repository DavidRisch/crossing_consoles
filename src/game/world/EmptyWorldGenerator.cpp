#include "EmptyWorldGenerator.h"

using namespace game;
using namespace game::common;
using namespace game::world;

std::shared_ptr<World> EmptyWorldGenerator::GenerateWorld(const coordinate_size_t& world_size) {
  auto world = std::make_shared<World>(world_size);

  return world;
}