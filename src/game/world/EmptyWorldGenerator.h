#ifndef CROSSING_CONSOLES_EMPTYWORLDGENERATOR_H
#define CROSSING_CONSOLES_EMPTYWORLDGENERATOR_H

#include "IWorldGenerator.h"
#include "World.h"

namespace game::world {

/**
 * \brief Used for generating empty world for testing.
 */
class EmptyWorldGenerator : public IWorldGenerator {
 public:
  /**
   * \brief Generates an empty world.
   */
  std::shared_ptr<World> GenerateWorld(const common::coordinate_size_t& world_size) override;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_EMPTYWORLDGENERATOR_H
