#ifndef CROSSING_CONSOLES_SWORD_H
#define CROSSING_CONSOLES_SWORD_H

#include <memory>

#include "IWeapon.h"

namespace game::world {

/**
 * \brief Short range item which causes damage to a `Player`.
 */
class Sword : public IWeapon {
 public:
  explicit Sword(int damage);

  /**
   * \brief Return the sprite of this item.
   */
  visual::ColoredCharMatrix GetSprite(common::coordinate_size_t block_size) override;

  /**
   * \brief Return the sprite of this item.
   */
  visual::ColoredCharMatrix GetItemBarSprite() override;

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

  /**
   * \brief Return the type of this weapon.
   */
  WeaponType GetWeaponType() override;

  /**
   * \brief Return the name of this weapon.
   */
  std::wstring GetName() override;

 private:
  int damage;
  const std::wstring name = L"Sword";
};
}  // namespace game::world
#endif  // CROSSING_CONSOLES_SWORD_H
