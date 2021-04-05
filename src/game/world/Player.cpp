#include "Player.h"

#include <cassert>
#include <utility>

#include "../networking/SerializationUtils.h"
#include "items/Weapon.h"

using namespace game;
using namespace game::common;
using namespace game::world;

Player::Player(std::string name, Position position, int player_id)
    : name(std::move(name))
    , position(std::move(position))
    , player_id(player_id) {
  // TODO Assign weapons dynamically by placing items in world
  weapon = Weapon(1, 20);  // dummy weapon
}

bool Player::IsAlive() const {
  return (health > 0);
}

void Player::MoveTo(const Position &new_position) {
  position = new_position;
  updated = true;
}

void Player::DecreaseHealth(int damage) {
  health -= damage;
}

void Player::Serialize(std::vector<uint8_t> &output_vector) const {
  output_vector.push_back(player_id);  // TODO: use 2 Bytes

  assert(name.size() <= 255);
  output_vector.push_back(name.size());
  for (const auto &character : name) {
    output_vector.push_back(character);
  }
  position.Serialize(output_vector);

  networking::SerializationUtils::SerializeObject(health, output_vector);
  networking::SerializationUtils::SerializeObject(score, output_vector);

  networking::SerializationUtils::SerializeObject(packet_loss_percentage, output_vector);
  networking::SerializationUtils::SerializeObject(ping, output_vector);
}

Player Player::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  int player_id = *input_iterator++;

  std::string name;
  auto name_length = *input_iterator++;
  for (int i = 0; i < name_length; ++i) {
    name += (char)*input_iterator++;
  }
  auto position = Position::Deserialize(input_iterator);

  Player player(name, position, player_id);

  player.health = networking::SerializationUtils::DeserializeObject<decltype(health)>(input_iterator);
  player.score = networking::SerializationUtils::DeserializeObject<decltype(score)>(input_iterator);

  player.packet_loss_percentage =
      networking::SerializationUtils::DeserializeObject<decltype(packet_loss_percentage)>(input_iterator);
  player.ping = networking::SerializationUtils::DeserializeObject<decltype(ping)>(input_iterator);

  return player;
}

std::optional<Weapon> Player::GetWeapon() {
  return weapon;
}

uint16_t Player::GetScore() const {
  return score;
}

void Player::IncreaseScore(uint16_t points) {
  score += points;
}
