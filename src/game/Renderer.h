#ifndef CROSSING_CONSOLES_RENDERER_H
#define CROSSING_CONSOLES_RENDERER_H

#include <string>

#include "Player.h"
#include "World.h"

class Renderer {
 public:
  const object_size_t block_size = Position(2, 1);
  object_size_t viewport_size;
  World* world;
  Player* player;

  explicit Renderer(int width, int height, World& world, Player& player);

  [[nodiscard]] std::string RenderWorld() const;
};

#endif  // CROSSING_CONSOLES_RENDERER_H
