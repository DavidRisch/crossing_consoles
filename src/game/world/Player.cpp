#include "Player.h"

#include <cassert>
#include <utility>

using namespace game;
using namespace game::common;
using namespace game::world;

Player::Player(std::string name, Position position, int player_id)
    : name(std::move(name))
    , position(std::move(position))
    , player_id(player_id) {
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
  output_vector.push_back(player_id);  // TODO: use 2 Bytes

  assert(name.size() <= 255);
  output_vector.push_back(name.size());
  for (const auto &character : name) {
    output_vector.push_back(character);
  }
  position.Serialize(output_vector);
}

Player Player::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  int player_id = *input_iterator++;

  std::string name;
  auto name_length = *input_iterator++;
  for (int i = 0; i < name_length; ++i) {
    name += (char)*input_iterator++;
  }
  auto position = Position::Deserialize(input_iterator);

  return Player(name, position, player_id);
}
