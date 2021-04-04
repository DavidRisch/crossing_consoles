#ifndef CROSSING_CONSOLES_WORLDGENERATOR_H
#define CROSSING_CONSOLES_WORLDGENERATOR_H

#include <memory>

#include "World.h"

namespace game::world {

/**
 * \brief Used to generate a World with some Walls.
 */
class WorldGenerator {
 public:
  static std::shared_ptr<World> GenerateWorld(const common::coordinate_size_t& world_size);
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_WORLDGENERATOR_H
