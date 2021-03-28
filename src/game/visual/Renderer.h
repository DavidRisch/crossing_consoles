#ifndef CROSSING_CONSOLES_RENDERER_H
#define CROSSING_CONSOLES_RENDERER_H

#include <string>

#include "../world/Player.h"
#include "../world/World.h"
#include "ColoredCharMatrix.h"

namespace game::visual {

class Renderer {
 public:
  common::coordinate_size_t block_size;
  common::coordinate_size_t viewport_size;
  world::World* world;
  world::Player* own_player;

  ColoredCharMatrix wall_sprite;
  ColoredCharMatrix player_sprite;

  explicit Renderer(common::coordinate_size_t viewport_size, common::coordinate_size_t block_size, world::World& world,
                    world::Player& own_player);

  [[nodiscard]] ColoredCharMatrix RenderWorld() const;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_RENDERER_H
