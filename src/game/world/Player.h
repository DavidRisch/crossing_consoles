#ifndef CROSSING_CONSOLES_PLAYER_H
#define CROSSING_CONSOLES_PLAYER_H

#include <string>

#include "../common/Position.h"

namespace game::world {

enum Direction : char { NORTH, EAST, SOUTH, WEST };

class Player : public networking::ISerializable {
 public:
  std::string name;
  int score = 0;
  common::Position position;
  Direction direction = NORTH;
  int max_health = 100;
  int health = max_health;
  bool updated = false;

  Player(std::string name, common::Position position);

  [[nodiscard]] bool IsAlive() const;

  void MoveTo(const common::Position& new_position);

  void Attack();

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Player Deserialize(std::vector<uint8_t>::iterator& input_iterator);
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_PLAYER_H
