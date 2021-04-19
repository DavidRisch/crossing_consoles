#ifndef CROSSING_CONSOLES_IITEM_H
#define CROSSING_CONSOLES_IITEM_H

#include "../../networking/ISerializable.h"
#include "../../visual/ColoredCharMatrix.h"

namespace game::world {

/**
 * \brief Possible item types.
 */
enum class ItemType : char { WEAPON, HEART, POINTS, HIGHEST_ELEMENT = POINTS };

/**
 * \brief Interface for game items.
 */
class IItem : public networking::ISerializable {
 public:
  virtual ~IItem() = default;
  virtual visual::ColoredCharMatrix GetSprite(common::coordinate_size_t block_size) = 0;
  virtual ItemType GetItemType() = 0;

};  // namespace game::world
}  // namespace game::world
#endif  // CROSSING_CONSOLES_IITEM_H
