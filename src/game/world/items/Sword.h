#ifndef CROSSING_CONSOLES_SWORD_H
#define CROSSING_CONSOLES_SWORD_H

#include <memory>

#include "IItem.h"

namespace game::world {

class Sword : public IItem {
 public:
  Sword(int damage, uint8_t max_range);

  void Display() override;
  void Serialize(std::vector<uint8_t>& output_vector) const override;
  static std::shared_ptr<Sword> Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  /**
   * \brief Return damage of this weapon
   */
  [[nodiscard]] int GetDamage() const;

  /**
   * \brief Return maximal range of projectiles spawned by this weapon
   */
  [[nodiscard]] uint8_t GetRange() const;

  ItemType GetItemType() override;

 private:
  int damage;
  uint8_t range;
  ItemType item_type;
};
}
#endif  // CROSSING_CONSOLES_SWORD_H
