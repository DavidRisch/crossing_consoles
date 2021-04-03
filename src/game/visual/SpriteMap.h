#ifndef CROSSING_CONSOLES_SPRITEMAP_H
#define CROSSING_CONSOLES_SPRITEMAP_H

#include <map>

#include "../common/Position.h"
#include "../world/block_types.h"
#include "ColoredCharMatrix.h"

namespace game::visual {

class SpriteMap {
 public:
  explicit SpriteMap(common::coordinate_size_t block_size);

  void SetSprite(world::BlockType type, const ColoredCharMatrix& sprite);
  const ColoredCharMatrix& GetSprite(world::BlockType type) const;

 private:
  std::map<world::BlockType, ColoredCharMatrix> type_sprites;
  common::coordinate_size_t block_size;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_SPRITEMAP_H
