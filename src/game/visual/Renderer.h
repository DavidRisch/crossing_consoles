#ifndef CROSSING_CONSOLES_RENDERER_H
#define CROSSING_CONSOLES_RENDERER_H

#include <functional>
#include <string>

#include "../world/Player.h"
#include "../world/World.h"
#include "SpriteMap.h"

namespace game::visual {

/**
 * \brief Renders the `World` as a `ColoredCharMatrix`.
 */
class Renderer {
 public:
  common::coordinate_size_t block_size;
  common::coordinate_size_t viewport_size;
  world::World* world;
  world::Player* own_player;
  SpriteMap sprite_map;

  explicit Renderer(common::coordinate_size_t viewport_size, common::coordinate_size_t block_size, world::World& world,
                    world::Player& own_player);

  [[nodiscard]] ColoredCharMatrix RenderWorld();

 private:
  common::coordinate_factor_t negative_repetition = common::coordinate_factor_t(0, 0);
  common::coordinate_factor_t positive_repetition = common::coordinate_factor_t(0, 0);
  common::Position viewport_start = common::Position(0, 0);
  common::Position viewport_end = common::Position(0, 0);

  static constexpr auto set_sprite = [](ColoredCharMatrix& matrix, const ColoredCharMatrix* sprite,
                                        const common::Position& position) { matrix.InsertMatrix(*sprite, position); };

  static constexpr auto color_field_red = [](ColoredCharMatrix& matrix, const ColoredCharMatrix* sprite,
                                             const common::Position& position) {
    matrix.SetBackgroundColorInRegion(const_cast<common::Position&>(position), sprite->GetSize(), common::Color::RED);
  };

  void RenderElement(const common::Position& position_element, const ColoredCharMatrix& element_sprite,
                     ColoredCharMatrix& rendered_world,
                     const std::function<void(game::visual::ColoredCharMatrix&, const game::visual::ColoredCharMatrix*,
                                              const game::common::Position&)>& change_world) const;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_RENDERER_H
