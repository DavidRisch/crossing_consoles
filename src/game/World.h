#ifndef CROSSING_CONSOLES_WORLD_H
#define CROSSING_CONSOLES_WORLD_H

#include <list>

#include "Player.h"
#include "Position.h"
#include "Wall.h"

class World {
 public:
  int size[2]{};
  std::list<Player> players;
  std::list<Wall> walls;

  World(int width, int height);

  bool IsWall(Position position);
};

#endif  // CROSSING_CONSOLES_WORLD_H
