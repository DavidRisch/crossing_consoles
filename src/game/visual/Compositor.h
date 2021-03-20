#ifndef CROSSING_CONSOLES_COMPOSITOR_H
#define CROSSING_CONSOLES_COMPOSITOR_H

#include <memory>

#include "../common/Position.h"
#include "../world/Player.h"
#include "../world/World.h"
#include "ColoredCharMatrix.h"
#include "Renderer.h"

namespace game::visual {

class Compositor {
 public:
  const common::coordinate_size_t block_size = common::coordinate_size_t(2, 2);
  common::coordinate_size_t viewport_size;
  std::unique_ptr<Renderer> renderer;
  world::World* world;
  world::Player* player;

  const common::coordinate_size_t composited_viewport_overhang = coordinate_size_t(2, 4);
  const common::coordinate_size_t rendered_viewport_offset = coordinate_size_t(1, 3);

  Compositor(common::coordinate_size_t viewport_size, world::World& world, world::Player& player);

  [[nodiscard]] ColoredCharMatrix CompositeViewport() const;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COMPOSITOR_H
