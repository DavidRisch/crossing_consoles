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
  world::Player* player;
  common::coordinate_size_t composited_viewport_overhang;
  common::coordinate_size_t rendered_viewport_offset;

  explicit Renderer(common::coordinate_size_t viewport_size, common::coordinate_size_t block_size, world::World& world,
                    world::Player& player,
                    common::coordinate_size_t composited_viewport_overhang = coordinate_size_t(0, 0),
                    common::coordinate_size_t rendered_viewport_offset = coordinate_size_t(0, 0));

  [[nodiscard]] ColoredCharMatrix RenderWorld() const;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_RENDERER_H
