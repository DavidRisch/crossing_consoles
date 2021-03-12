#ifndef CROSSING_CONSOLES_WORLDGENERATOR_H
#define CROSSING_CONSOLES_WORLDGENERATOR_H

#include "World.h"

class Generator {
 public:
  Generator();

  World GenerateWorld(coordinate_size_t size);
};

#endif  // CROSSING_CONSOLES_WORLDGENERATOR_H
