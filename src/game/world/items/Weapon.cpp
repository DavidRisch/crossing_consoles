#include "Weapon.h"

#include <utility>

using namespace game;
using namespace game::world;

int Weapon::GetDamage() const {
  return damage;
}

uint8_t Weapon::GetRange() const {
  return range;
}

Weapon::Weapon(int damage, uint8_t max_range)
    : damage(damage)
    , range(max_range)
    ,item_type(ItemType::LONG_RANGE){
}

Projectile Weapon::SpawnProjectile(GameDefinition::player_id_t shooter_id, common::Position position,
                                   GameDefinition::Direction direction) const {
  // range is increased by 1 as projectile spawns at player's position
  return Projectile(range + 1, damage, shooter_id, std::move(position), direction);
}

void Weapon::Serialize(std::vector<uint8_t> &output_vector) const {
  output_vector.push_back(damage);
  output_vector.push_back(range);
}

std::shared_ptr<Weapon> Weapon::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  int damage = *input_iterator++;
  uint8_t range = *input_iterator++;

  auto weapon = std::make_shared<Weapon>(damage, range);
  return weapon;
}

ItemType Weapon::GetItemType() {
  return item_type;
}

void Weapon::Display() {
}
