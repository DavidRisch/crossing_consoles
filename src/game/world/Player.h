#ifndef CROSSING_CONSOLES_PLAYER_H
#define CROSSING_CONSOLES_PLAYER_H

#include <chrono>
#include <string>

#include "../GameDefinition.h"
#include "../common/Position.h"
#include "items/Weapon.h"

namespace game::world {

class Player : public networking::ISerializable {
 public:
  std::string name;
  int score = 0;
  common::Position position;
  GameDefinition::Direction direction = GameDefinition::NORTH;
  static constexpr int max_health = 8;
  int health = max_health;
  bool updated = false;
  double packet_loss_percentage = 0;
  std::optional<std::chrono::microseconds> ping{};

  Player(std::string name, common::Position position, int player_id = 999);

  [[nodiscard]] bool IsAlive() const;

  void MoveTo(const common::Position& new_position);

  /**
   * \brief Decrease player's health by given `damage`.
   */
  void DecreaseHealth(int damage);

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Player Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  GameDefinition::player_id_t player_id;

  /**
   * \brief Returns the player's `Weapon'
   */
  std::optional<Weapon> GetWeapon();

 private:
  std::optional<Weapon> weapon;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_PLAYER_H
