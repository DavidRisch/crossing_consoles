#ifndef CROSSING_CONSOLES_WORLDGENERATOR_H
#define CROSSING_CONSOLES_WORLDGENERATOR_H

#include <memory>

#include "World.h"

class WorldGenerator {
 public:
  static std::shared_ptr<World> GenerateWorld(const coordinate_size_t& world_size);
};

#endif  // CROSSING_CONSOLES_WORLDGENERATOR_H
