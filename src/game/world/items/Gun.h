#ifndef CROSSING_CONSOLES_GUN_H
#define CROSSING_CONSOLES_GUN_H

#include <memory>

#include "../../GameDefinition.h"
#include "../Projectile.h"
#include "IWeapon.h"

namespace game::world {

class Gun : public IWeapon {
 public:
  Gun(int damage, uint8_t max_range);

  /**
   * \brief Create Projectile with damage and range given by weapon
   * \details Initialize with position and direction of `Player` with `shooter_id`
   */
  [[nodiscard]] Projectile SpawnProjectile(GameDefinition::player_id_t shooter_id, common::Position position,
                                           GameDefinition::Direction direction) const;

  /**
   * \brief Return damage of projectiles spawned by this weapon.
   */
  [[nodiscard]] int GetDamage() const;

  /**
   * \brief Return maximal range of projectiles spawned by this weapon.
   */
  [[nodiscard]] uint8_t GetRange() const;

  /**
   * \brief Serialize the item for transmission over the network.
   */
  void Serialize(std::vector<uint8_t> &output_vector) const override;

  /**
   * \brief Deserialize the item after receiving it over the network.
   */
  static std::shared_ptr<Gun> Deserialize(std::vector<uint8_t>::iterator &input_iterator);

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

  /**
   * \brief Return the sprite of this item.
   */
  visual::ColoredCharMatrix GetSprite(common::coordinate_size_t block_size) override;

  /**
   * \brief Return the item bar sprite of this item.
   */
  visual::ColoredCharMatrix GetItemBarSprite() override;

 private:
  int damage;
  uint8_t range;
  const std::wstring name = L"Gun";
};
}  // namespace game::world

#endif  // CROSSING_CONSOLES_GUN_H
