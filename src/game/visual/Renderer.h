#ifndef CROSSING_CONSOLES_RENDERER_H
#define CROSSING_CONSOLES_RENDERER_H

#include <string>

#include "../world/Player.h"
#include "../world/World.h"
#include "ColoredStringMatrix.h"

namespace game::visual {

class Renderer {
 public:
  common::coordinate_size_t block_size;
  common::coordinate_size_t viewport_size;
  common::coordinate_size_t composited_viewport_overhang;
  common::coordinate_size_t rendered_viewport_offset;
  world::World* world;
  world::Player* player;

  explicit Renderer(common::coordinate_size_t viewport_size, common::coordinate_size_t block_size,
                    common::coordinate_size_t composited_viewport_overhang,
                    common::coordinate_size_t rendered_viewport_offset, world::World& world, world::Player& player);

  [[nodiscard]] ColoredStringMatrix RenderWorld() const;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_RENDERER_H
