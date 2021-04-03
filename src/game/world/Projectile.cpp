#include "Projectile.h"

#include <utility>

#include "../networking/SerializationUtils.h"

using namespace game;
using namespace game::common;

Projectile::Projectile(uint8_t range, int damage, GameDefinition::player_id_t shooter_id, Position position,
                       GameDefinition::Direction direction)
    : range(range)
    , damage(damage)
    , shooter_id(shooter_id)
    , position(std::move(position))
    , direction(direction) {
}

bool Projectile::StillFlying() const {
  return range > 0;
}

void Projectile::DecreaseRange() {
  if (range > 0) {
    range -= 1;
  } else {
    range = 0;
  }
}
int Projectile::GetDamage() const {
  return damage;
}
GameDefinition::player_id_t Projectile::GetShooterId() const {
  return shooter_id;
}

common::Position Projectile::GetPosition() const {
  return position;
}

GameDefinition::Direction Projectile::GetDirection() const {
  return direction;
}
void Projectile::Serialize(std::vector<uint8_t>& output_vector) const {
  SerializationUtils::SerializeObject(range, output_vector);
  SerializationUtils::SerializeObject(damage, output_vector);
  SerializationUtils::SerializeObject(shooter_id, output_vector);
  SerializationUtils::SerializeObject(direction, output_vector);
  position.Serialize(output_vector);
}

Projectile Projectile::Deserialize(std::vector<uint8_t>::iterator& input_iterator) {
  auto deserialized_range = SerializationUtils::DeserializeObject<decltype(range)>(input_iterator);
  auto deserialized_damage = SerializationUtils::DeserializeObject<decltype(damage)>(input_iterator);
  auto deserialized_id = SerializationUtils::DeserializeObject<decltype(shooter_id)>(input_iterator);
  auto deserialized_direction = SerializationUtils::DeserializeObject<decltype(direction)>(input_iterator);
  Position deserialized_position = Position::Deserialize(input_iterator);

  return Projectile(deserialized_range, deserialized_damage, deserialized_id, deserialized_position,
                    deserialized_direction);
}
void Projectile::SetPosition(common::Position new_position, GameDefinition::Direction new_direction) {
  position = std::move(new_position);
  direction = new_direction;
}

uint8_t Projectile::GetRange() const {
  return range;
}
