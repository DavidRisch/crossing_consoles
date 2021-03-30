#ifndef CROSSING_CONSOLES_COMPOSITOR_H
#define CROSSING_CONSOLES_COMPOSITOR_H

#include <memory>

#include "../common/Position.h"
#include "../world/Player.h"
#include "../world/World.h"
#include "ColoredCharMatrix.h"
#include "PlayerList.h"
#include "Renderer.h"

namespace game::visual {

class Compositor {
 public:
  const common::coordinate_size_t block_size = common::coordinate_size_t(2, 2);
  const common::coordinate_size_t composited_viewport_overhang = common::coordinate_size_t(2, 4);
  const common::coordinate_size_t rendered_viewport_offset = common::coordinate_size_t(1, 3);

  common::coordinate_size_t viewport_size;
  std::unique_ptr<Renderer> renderer;
  world::World* world;
  world::Player* player;
  bool show_player_list = false;

  Compositor(const common::coordinate_size_t& viewport_size, world::World& world, world::Player& player);

  [[nodiscard]] ColoredCharMatrix CompositeViewport() const;

 private:
  std::unique_ptr<PlayerList> player_list;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COMPOSITOR_H
