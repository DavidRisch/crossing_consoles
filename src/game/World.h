#ifndef CROSSING_CONSOLES_WORLD_H
#define CROSSING_CONSOLES_WORLD_H

#include <list>

#include "Player.h"
#include "Position.h"
#include "Wall.h"

class World {
 public:
  coordinate_size_t size;
  std::list<Player*> players{};
  std::list<Wall*> walls{};
  bool updated = false;

  World(int width, int height);

  void AddPlayer(Player* player);
  void AddWall(const Position& position);

  bool IsBlocked(const Position& position);
};

#endif  // CROSSING_CONSOLES_WORLD_H
