#include "Gun.h"

#include <utility>

using namespace game;
using namespace game::world;

int Gun::GetDamage() const {
  return damage;
}

uint8_t Gun::GetRange() const {
  return range;
}

Gun::Gun(int damage, uint8_t max_range)
    : damage(damage)
    , range(max_range)
    , item_type(ItemType::GUN) {
}

Projectile Gun::SpawnProjectile(GameDefinition::player_id_t shooter_id, common::Position position,
                                GameDefinition::Direction direction) const {
  // range is increased by 1 as projectile spawns at player's position
  return Projectile(range + 1, damage, shooter_id, std::move(position), direction);
}

void Gun::Serialize(std::vector<uint8_t> &output_vector) const {
  output_vector.push_back(damage);
  output_vector.push_back(range);
}

std::shared_ptr<Gun> Gun::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  int damage = *input_iterator++;
  uint8_t range = *input_iterator++;

  auto weapon = std::make_shared<Gun>(damage, range);
  return weapon;
}

ItemType Gun::GetItemType() {
  return item_type;
}

visual::ColoredCharMatrix Gun::GetSprite(common::coordinate_size_t block_size) {
  visual::ColoredCharMatrix colored_char_matrix = visual::ColoredCharMatrix(block_size);
  colored_char_matrix.AppendString(L"OOOO");
  return colored_char_matrix;
}
