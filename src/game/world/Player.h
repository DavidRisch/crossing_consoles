#ifndef CROSSING_CONSOLES_PLAYER_H
#define CROSSING_CONSOLES_PLAYER_H

#include <chrono>
#include <string>

#include "../GameDefinition.h"
#include "../common/Position.h"
#include "items/Weapon.h"

namespace game::world {

/**
 * \brief Representation of a player.
 * \details Representation of a player with name, position, direction, health and information about his connection.
 */
class Player : public networking::ISerializable {
 public:
  std::string name;
  common::Position position;
  GameDefinition::player_id_t player_id;
  GameDefinition::Direction direction;

  static constexpr int max_health = 8;
  int health = max_health;
  bool updated = false;
  double packet_loss_percentage = 0;
  std::optional<std::chrono::microseconds> ping{};
  std::chrono::steady_clock::time_point time_of_death;
  uint16_t score = 0;

  Player(std::string name, common::Position position, int player_id = 999,
         GameDefinition::Direction direction = GameDefinition::NORTH);

  [[nodiscard]] bool IsAlive() const;

  /**
   * \brief Set player's time of Death
   * \details Time of death is used to determine respawn.
   */
  void Die();

  void MoveTo(const common::Position& new_position);

  /**
   * \brief Decrease player's health by given `damage`.
   */
  void DecreaseHealth(int damage);

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Player Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  [[nodiscard]] uint16_t GetScore() const;

  /**
   * \brief Increase player's score by given `points`.
   */
  void IncreaseScore(uint16_t points);

  /**
   * \brief Returns the player's `Weapon'
   */
  std::shared_ptr<IItem> GetItem();

  void SetItem(std::shared_ptr<IItem> new_item);

 private:
  std::shared_ptr<IItem> item;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_PLAYER_H
