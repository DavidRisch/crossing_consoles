#ifndef CROSSING_CONSOLES_PLAYER_H
#define CROSSING_CONSOLES_PLAYER_H

#include <string>

#include "Position.h"

enum Direction : char { NORTH, EAST, SOUTH, WEST };

class Player {
 public:
  std::string name;
  int score = 0;
  Position position;
  Direction direction = NORTH;
  static int max_health;
  int health = max_health;

  explicit Player(std::string name, Position position);

  [[nodiscard]] bool IsAlive() const;

  void Move(const Position& new_position);

  void Attack();
};

#endif  // CROSSING_CONSOLES_PLAYER_H
