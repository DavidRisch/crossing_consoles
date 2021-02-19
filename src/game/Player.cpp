#include "Player.h"

#include <utility>

Player::Player(std::string name, Position position)
    : position(position)
    , name(std::move(name)) {
}

bool Player::IsAlive() const {
  return (health > 0);
}

void Player::Move(const Position &new_position) {
}

void Player::Attack() {
}