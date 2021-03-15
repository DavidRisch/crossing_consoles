#include "Player.h"

#include <cassert>
#include <utility>

using namespace game;
using namespace game::common;
using namespace game::world;

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

void Player::Serialize(std::vector<uint8_t> &output_vector) const {
  assert(name.size() <= 255);
  output_vector.push_back(name.size());
  for (const auto &character : name) {
    output_vector.push_back(character);
  }
  position.Serialize(output_vector);
}

Player Player::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  std::string name;
  auto name_length = *input_iterator++;
  for (int i = 0; i < name_length; ++i) {
    name += (char)*input_iterator++;
  }
  auto position = Position::Deserialize(input_iterator);

  return Player(name, position);
}
