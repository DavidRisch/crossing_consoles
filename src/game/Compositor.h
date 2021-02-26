#ifndef CROSSING_CONSOLES_COMPOSITOR_H
#define CROSSING_CONSOLES_COMPOSITOR_H

#include "Player.h"
#include "Position.h"
#include "Renderer.h"
#include "World.h"

class Compositor {
 public:
  const coordinate_size_t block_size = Position(2, 2);
  coordinate_size_t viewport_size;
  Renderer* renderer;
  World* world;
  Player* player;

  Compositor(coordinate_size_t viewport_size, World& world, Player& player);

  [[nodiscard]] std::string CompositeViewport() const;
};

#endif  // CROSSING_CONSOLES_COMPOSITOR_H
