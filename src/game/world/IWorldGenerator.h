#ifndef CROSSING_CONSOLES_I_WORLDGENERATOR_H
#define CROSSING_CONSOLES_I_WORLDGENERATOR_H

#include "../common/Position.h"
#include "World.h"

namespace game::world {

/**
 * \brief Interface for generating worlds.
 */
class IWorldGenerator {
 public:
  virtual ~IWorldGenerator() = default;

  /**
   * \brief Generates a new world.
   */
  virtual std::shared_ptr<World> GenerateWorld(const common::coordinate_size_t& world_size) = 0;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_I_WORLDGENERATOR_H
