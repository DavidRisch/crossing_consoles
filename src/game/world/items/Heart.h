#ifndef CROSSING_CONSOLES_HEART_H
#define CROSSING_CONSOLES_HEART_H

#include <memory>

#include "IItem.h"

namespace game::world {

/**
 * \brief Item which increases a `Player`s health.
 */
class Heart : public IItem {
 public:
  explicit Heart(int healing);

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
  static std::shared_ptr<Heart> Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  /**
   * \brief Return healing of this item.
   */
  [[nodiscard]] int GetHealing() const;

  /**
   * \brief Return the type of this item.
   */
  ItemType GetItemType() override;

 private:
  int healing;
  ItemType item_type;
};
}  // namespace game::world

#endif  // CROSSING_CONSOLES_HEART_H
