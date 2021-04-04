#ifndef CROSSING_CONSOLES_PROJECTILE_H
#define CROSSING_CONSOLES_PROJECTILE_H

#include "../GameDefinition.h"
#include "../common/Position.h"
#include "../networking/ISerializable.h"

namespace game {

class Projectile : public networking::ISerializable {
  /**
   * \brief Projectiles are moving objects of `World` with a certain range and damage
   */

 public:
  Projectile(uint8_t range, int damage, GameDefinition::player_id_t shooter_id, common::Position position,
             GameDefinition::Direction direction);
  /**
   * \brief Decreases projectile's range.
   */
  void DecreaseRange();

  /**
   * \brief True if projectile's range is greater than 0.
   */
  [[nodiscard]] bool StillFlying() const;

  /**
   * \brief Returns Id of player that spawned the projectile.
   */
  [[nodiscard]] GameDefinition::player_id_t GetShooterId() const;

  [[nodiscard]] common::Position GetPosition() const;

  [[nodiscard]] GameDefinition::Direction GetDirection() const;

  [[nodiscard]] uint8_t GetRange() const;

  [[nodiscard]] int GetDamage() const;

  void SetPosition(common::Position new_position, GameDefinition::Direction new_direction);

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Projectile Deserialize(std::vector<uint8_t>::iterator& input_iterator);

 private:
  uint8_t range;
  uint8_t damage;
  const GameDefinition::player_id_t shooter_id;
  common::Position position;
  GameDefinition::Direction direction;
};

}  // namespace game

#endif  // CROSSING_CONSOLES_PROJECTILE_H
