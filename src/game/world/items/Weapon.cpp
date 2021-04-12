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
    , range(max_range) {
}

Projectile Weapon::SpawnProjectile(GameDefinition::player_id_t shooter_id, common::Position position,
                                   GameDefinition::Direction direction) const {
  // range is increased by 1 as projectile spawns at player's position
  return Projectile(range + 1, damage, shooter_id, std::move(position), direction);
}
