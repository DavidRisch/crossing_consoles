#ifndef CROSSING_CONSOLES_IITEM_H
#define CROSSING_CONSOLES_IITEM_H

#include "../../networking/ISerializable.h"
#include "../../visual/ColoredCharMatrix.h"

namespace game::world {

enum class ItemType : char {
  LONG_RANGE,
  SWORD,
  HEALING,
  POINTS,
  HIGHEST_ELEMENT = POINTS
};

class IItem : public networking::ISerializable {
 public:
  virtual visual::ColoredCharMatrix GetSprite(common::coordinate_size_t block_size) = 0;
  virtual ItemType GetItemType() = 0;
};

}

#endif  // CROSSING_CONSOLES_IITEM_H
