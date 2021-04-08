#include "Sword.h"

#include <memory>

using namespace game;
using namespace world;

visual::ColoredCharMatrix Sword::GetSprite(common::coordinate_size_t block_size) {
  visual::ColoredCharMatrix colored_char_matrix = visual::ColoredCharMatrix(block_size);
  colored_char_matrix.AppendString(L" // ");
  return colored_char_matrix;
}

void Sword::Serialize(std::vector<uint8_t> &output_vector) const {
  output_vector.push_back(damage);
  output_vector.push_back(range);
}

std::shared_ptr<Sword> Sword::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  int new_damage = *input_iterator++;
  uint8_t new_range = *input_iterator++;

  auto sword = std::make_shared<Sword>(new_damage, new_range);
  return sword;
}

Sword::Sword(int damage, uint8_t max_range)
    : damage(damage)
    , range(max_range)
    , item_type(ItemType::SWORD) {
}

int Sword::GetDamage() const {
  return damage;
}

uint8_t Sword::GetRange() const {
  return range;
}

ItemType Sword::GetItemType() {
  return item_type;
}
