#ifndef CROSSING_CONSOLES_PLAYER_H
#define CROSSING_CONSOLES_PLAYER_H

#include <chrono>
#include <string>

#include "../GameDefinition.h"
#include "../common/Position.h"

namespace game::world {

enum Direction : char { NORTH, EAST, SOUTH, WEST };

class Player : public networking::ISerializable {
 public:
  std::string name;
  int score = 0;
  common::Position position;
  Direction direction = NORTH;
  static constexpr int max_health = 8;
  int health = max_health;
  bool updated = false;
  double packet_loss_percentage = 0;
  std::optional<std::chrono::microseconds> ping{};

  Player(std::string name, common::Position position, int player_id = 999);

  [[nodiscard]] bool IsAlive() const;

  void MoveTo(const common::Position& new_position);

  void Attack();

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Player Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  GameDefinition::player_id_t player_id;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_PLAYER_H
