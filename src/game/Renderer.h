#ifndef CROSSING_CONSOLES_RENDERER_H
#define CROSSING_CONSOLES_RENDERER_H

#include <string>

#include "Player.h"
#include "World.h"

class Renderer {
 public:
  coordinate_size_t block_size;
  coordinate_size_t viewport_size;
  World* world;
  Player* player;

  explicit Renderer(coordinate_size_t viewport_size, coordinate_size_t block_size, World& world, Player& player);

  [[nodiscard]] std::string RenderWorld() const;
};

#endif  // CROSSING_CONSOLES_RENDERER_H