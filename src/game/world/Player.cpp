#include "Player.h"

#include <utility>

#include "../networking/SerializationUtils.h"
#include "items/DeserializeItemUtils.h"
#include "items/Gun.h"
#include "items/Heart.h"
#include "items/Points.h"
#include "items/Sword.h"

using namespace game;
using namespace game::common;
using namespace game::world;

Player::Player(std::string name, Position position)
    : Player(std::move(name), Color::RED, std::move(position), GameDefinition::Direction::NORTH, 999) {
}

Player::Player(std::string name, common::Color color, Position position, GameDefinition::Direction direction,
               int player_id)
    : name(std::move(name))
    , color(std::move(color))
    , position(std::move(position))
    , player_id(player_id)
    , direction(direction) {
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

  networking::SerializationUtils::SerializeString(name, output_vector);
  color.Serialize(output_vector);
  position.Serialize(output_vector);

  networking::SerializationUtils::SerializeObject(direction, output_vector);

  if (item != nullptr) {
    networking::SerializationUtils::SerializeObject(true, output_vector);
    networking::SerializationUtils::SerializeObject(item->GetItemType(), output_vector);
    item->Serialize(output_vector);
  } else {
    networking::SerializationUtils::SerializeObject(false, output_vector);
  }

  networking::SerializationUtils::SerializeObject(health, output_vector);
  networking::SerializationUtils::SerializeObject(score, output_vector);

  networking::SerializationUtils::SerializeObject(packet_loss_percentage, output_vector);
  networking::SerializationUtils::SerializeObject(ping, output_vector);
}

Player Player::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  int player_id = *input_iterator++;

  auto name = networking::SerializationUtils::DeserializeString(input_iterator);
  auto color = Color::Deserialize(input_iterator);
  auto position = Position::Deserialize(input_iterator);
  auto direction = networking::SerializationUtils::DeserializeObject<GameDefinition::Direction>(input_iterator);

  Player player(name, color, position, direction, player_id);

  // Deserialize item of the player, either Gun or Sword
  bool has_item = networking::SerializationUtils::DeserializeObject<bool>(input_iterator);
  if (has_item) {
    auto item_type = networking::SerializationUtils::DeserializeObject<ItemType>(input_iterator);
    auto new_item = game::world::DeserializeItemUtils::DeserializeItem(item_type, input_iterator);
    player.SetItem(new_item);
  }

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

void Player::SetItem(std::shared_ptr<IItem> new_item) {
  switch (new_item->GetItemType()) {
    case ItemType::HEART:
      DecreaseHealth(-std::dynamic_pointer_cast<Heart>(new_item)->GetHealing());
      break;
    case ItemType::POINTS:
      IncreaseScore(std::dynamic_pointer_cast<Points>(new_item)->GetValue());
      break;
    case ItemType::GUN:
    case ItemType::SWORD:
      item = std::move(new_item);
      break;
  }
}

void Player::Die() {
  time_of_death = std::chrono::steady_clock::now();
}

void Player::RemoveItem() {
  item.reset();
}
