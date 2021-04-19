#ifndef CROSSING_CONSOLES_SPRITE_H
#define CROSSING_CONSOLES_SPRITE_H

#include <map>
#include <utility>

#include "../GameDefinition.h"
#include "../common/Color.h"
#include "../common/Position.h"
#include "../world/block_types.h"
#include "ColoredCharMatrix.h"
#include "SpriteMap.h"
#include "symbols.h"

namespace game::visual {

/**
 * \brief Generate all used sprites in `world` as `ColoredCharMatrix` and associate them with `BlockTypes`.
 */
class SpriteGenerator {
 public:
  explicit SpriteGenerator(common::coordinate_size_t block_size);

  /**
   * \brief Initializes a mapping of `BlockType` to sprites in form of `ColoredCharMatrix`.
   */
  game::visual::SpriteMap InitializeMap();

  static ColoredCharMatrix GenerateItemSprite(const std::wstring& string, const common::coordinate_size_t& block_size,
                                              const common::Color& foreground_color = common::Color::WHITE,
                                              const common::Color& background_color = common::Color::BLACK);

  static ColoredCharMatrix GeneratePointSprite(int value, const common::coordinate_size_t& block_size,
                                               const common::Color& foreground_color = common::Color::WHITE,
                                               const common::Color& background_color = common::Color::BLACK);

 private:
  /**
   * \brief Generate a `ColoredCharMatrix` using the specified wall sprite parameters.
   */
  ColoredCharMatrix GenerateWallSprite(wchar_t type, const common::Color& foreground_color = common::Color::WHITE,
                                       const common::Color& background_color = common::Color::BLACK);

  /**
   * \brief Generate a `ColoredCharMatrix` of the specified player sprite.
   */
  ColoredCharMatrix GeneratePlayerSprite(world::BlockType block_type);

  /**
   * \brief Generate the `ColoredCharMatrix`  for the projectile sprite.
   */
  ColoredCharMatrix GenerateProjectileSprite();

  common::coordinate_size_t block_size;
};
}  // namespace game::visual
#endif  // CROSSING_CONSOLES_SPRITE_H
