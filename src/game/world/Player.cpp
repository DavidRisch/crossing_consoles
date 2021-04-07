#include "Player.h"

#include <cassert>
#include <utility>

#include "../networking/SerializationUtils.h"
#include "items/Sword.h"
#include "items/Weapon.h"

using namespace game;
using namespace game::common;
using namespace game::world;

Player::Player(std::string name, Position position, int player_id, GameDefinition::Direction direction)
    : name(std::move(name))
    , position(std::move(position))
    , player_id(player_id)
    , direction(direction) {
  // TODO Assign weapons dynamically by placing items in world
  item = std::make_shared<Weapon>(1, 20);  // dummy weapon
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

  networking::SerializationUtils::SerializeObject(direction, output_vector);

  networking::SerializationUtils::SerializeObject(item->GetItemType(), output_vector);
  item->Serialize(output_vector);

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
  auto direction = networking::SerializationUtils::DeserializeObject<GameDefinition::Direction>(input_iterator);

  auto item_type = networking::SerializationUtils::DeserializeObject<ItemType>(input_iterator);
  auto new_item = std::shared_ptr<IItem>();
  switch (item_type)
  {
    case ItemType::SWORD:
      new_item = Sword::Deserialize(input_iterator);
      break;
    case ItemType::LONG_RANGE:
      new_item = Weapon::Deserialize(input_iterator);
      break;
    case ItemType::HEALING: // TODO
      break;
    case ItemType::POINTS:
      break;
  }

  Player player(name, position, player_id, direction);
  player.SetItem(new_item);

  player.health = networking::SerializationUtils::DeserializeObject<decltype(health)>(input_iterator);
  player.score = networking::SerializationUtils::DeserializeObject<decltype(score)>(input_iterator);

  player.packet_loss_percentage =
      networking::SerializationUtils::DeserializeObject<decltype(packet_loss_percentage)>(input_iterator);
  player.ping = networking::SerializationUtils::DeserializeObject<decltype(ping)>(input_iterator);

  return player;
}

std::shared_ptr<IItem> Player::GetItem() {
  return item;
}

uint16_t Player::GetScore() const {
  return score;
}

void Player::IncreaseScore(uint16_t points) {
  score += points;
}

void Player::SetItem(std::shared_ptr<IItem> new_item){
  item = std::move(new_item);
}