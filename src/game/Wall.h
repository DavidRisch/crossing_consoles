#ifndef CROSSING_CONSOLES_WALL_H
#define CROSSING_CONSOLES_WALL_H

#include "Position.h"

class Wall {
 public:
  Position position;

  explicit Wall(Position position);
};

#endif  // CROSSING_CONSOLES_WALL_H
