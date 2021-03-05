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
  int max_health = 100;
  int health = max_health;
  bool updated = false;

  Player(std::string name, Position position);

  [[nodiscard]] bool IsAlive() const;

  void MoveTo(const Position& new_position);

  void Attack();
};

#endif  // CROSSING_CONSOLES_PLAYER_H
