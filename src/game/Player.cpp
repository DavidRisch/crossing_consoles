#include "Player.h"

#include <cassert>
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

void Player::Attack() {
}

void Player::Serialize(std::vector<uint8_t> &into) const {
  assert(name.size() <= 255);
  into.push_back(name.size());
  for (const auto &character : name) {
    into.push_back(character);
  }
  position.Serialize(into);
}

Player Player::Deserialize(std::vector<uint8_t>::iterator &from) {
  std::string name;
  auto name_length = *from++;
  for (int i = 0; i < name_length; ++i) {
    name += (char)*from++;
  }
  auto position = Position::Deserialize(from);

  return Player(name, position);
}
