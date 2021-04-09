#ifndef CROSSING_CONSOLES_POINTS_H
#define CROSSING_CONSOLES_POINTS_H

#include <memory>

#include "IItem.h"

namespace game::world {

/**
 * \brief Items which increases a `Player`s score.
 */
class Points : public IItem {
 public:
  explicit Points(uint8_t value);

  /**
   * \brief Return the sprite of this item.
   */
  visual::ColoredCharMatrix GetSprite(common::coordinate_size_t block_size) override;

  /**
   * \brief Serialize the item for transmission over the network.
   */
  void Serialize(std::vector<uint8_t>& output_vector) const override;

  /**
   * \brief Deserialize the item after receiving it over the network.
   */
  static std::shared_ptr<Points> Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  /**
   * \brief Return the amount of points the item is worth.
   */
  uint8_t GetValue() const;

  /**
   * \brief Return the type of this item.
   */
  ItemType GetItemType() override;

 private:
  uint8_t value;
  ItemType item_type;
};
}  // namespace game::world

#endif  // CROSSING_CONSOLES_POINTS_H
