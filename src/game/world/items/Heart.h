#ifndef CROSSING_CONSOLES_HEART_H
#define CROSSING_CONSOLES_HEART_H

#include <memory>

#include "IItem.h"

namespace game::world {

class Heart : public IItem {
 public:
  explicit Heart(int damage);

  visual::ColoredCharMatrix GetSprite(common::coordinate_size_t block_size) override;
  void Serialize(std::vector<uint8_t>& output_vector) const override;
  static std::shared_ptr<Heart> Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  /**
   * \brief Return damage of this weapon, will be negative for healing
   */
  [[nodiscard]] int GetDamage() const;

  ItemType GetItemType() override;

 private:
  int damage;
  ItemType item_type;
};
}  // namespace game::world

#endif  // CROSSING_CONSOLES_HEART_H
