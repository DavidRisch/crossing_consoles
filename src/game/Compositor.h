#ifndef CROSSING_CONSOLES_COMPOSITOR_H
#define CROSSING_CONSOLES_COMPOSITOR_H

#include <memory>

#include "Player.h"
#include "Position.h"
#include "Renderer.h"
#include "World.h"

class Compositor {
 public:
  const coordinate_size_t block_size = coordinate_size_t(2, 2);
  coordinate_size_t viewport_size;
  std::unique_ptr<Renderer> renderer;
  World* world;
  Player* player;

  Compositor(coordinate_size_t viewport_size, World& world, Player& player);

  [[nodiscard]] std::wstring CompositeViewport() const;
};

#endif  // CROSSING_CONSOLES_COMPOSITOR_H
