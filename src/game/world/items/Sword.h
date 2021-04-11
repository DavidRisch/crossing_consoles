#ifndef CROSSING_CONSOLES_SWORD_H
#define CROSSING_CONSOLES_SWORD_H

#include <memory>

#include "IItem.h"

namespace game::world {

/**
 * \brief Short range item which causes damage to a `Player`.
 */
class Sword : public IItem {
 public:
  explicit Sword(int damage);

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
  static std::shared_ptr<Sword> Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  /**
   * \brief Return damage of this weapon.
   */
  [[nodiscard]] int GetDamage() const;

  /**
   * \brief Return the type of this item.
   */
  ItemType GetItemType() override;

 private:
  int damage;
};
}  // namespace game::world
#endif  // CROSSING_CONSOLES_SWORD_H
