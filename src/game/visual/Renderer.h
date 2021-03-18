#ifndef CROSSING_CONSOLES_RENDERER_H
#define CROSSING_CONSOLES_RENDERER_H

#include <string>

#include "../world/Player.h"
#include "../world/World.h"

namespace game::visual {

class Renderer {
 public:
  common::coordinate_size_t block_size;
  common::coordinate_size_t viewport_size;
  int line_length = block_size.x * viewport_size.x + 1;
  world::World* world;
  world::Player* own_player;

  explicit Renderer(common::coordinate_size_t viewport_size, common::coordinate_size_t block_size, world::World& world,
                    world::Player& own_player);

  [[nodiscard]] std::wstring RenderWorld() const;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_RENDERER_H
