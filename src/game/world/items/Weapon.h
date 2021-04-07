#ifndef CROSSING_CONSOLES_WEAPON_H
#define CROSSING_CONSOLES_WEAPON_H

#include <memory>

#include "../../GameDefinition.h"
#include "../Projectile.h"
#include "IItem.h"

namespace game::world {

class Weapon : public IItem {
 public:
  Weapon(int damage, uint8_t max_range);

  /**
   * \brief Create Projectile with damage and range given by weapon
   * \details Initialize with position and direction of `Player` with `shooter_id`
   */
  [[nodiscard]] Projectile SpawnProjectile(GameDefinition::player_id_t shooter_id, common::Position position,
                                           GameDefinition::Direction direction) const;

  /**
   * \brief Return damage of projectiles spawned by this weapon
   */
  [[nodiscard]] int GetDamage() const;

  /**
   * \brief Return maximal range of projectiles spawned by this weapon
   */
  [[nodiscard]] uint8_t GetRange() const;

  void Serialize(std::vector<uint8_t> &output_vector) const override;

  static std::shared_ptr<Weapon> Deserialize(std::vector<uint8_t>::iterator &input_iterator);

  ItemType GetItemType() override;

  void Display() override;

 private:
  int damage;
  uint8_t range;
  ItemType item_type;
};
}  // namespace game::world

#endif  // CROSSING_CONSOLES_WEAPON_H
