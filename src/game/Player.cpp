#include "Player.h"

#include <utility>

Player::Player(std::string name, Position position)
    : name(std::move(name))
    , position(position) {
}

bool Player::IsAlive() const {
  return (health > 0);
}

void Player::MoveTo(const Position &new_position) {
  position = new_position;
  updated = true;
}

void Player::MoveBy(const Position &new_position_diff) {
  position = position + new_position_diff;
  updated = true;
}

void Player::Attack() {
}