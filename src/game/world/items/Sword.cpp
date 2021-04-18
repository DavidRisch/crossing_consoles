#include "Sword.h"

#include <memory>

#include "../../visual/symbols.h"

using namespace game;
using namespace world;

visual::ColoredCharMatrix Sword::GetSprite(common::coordinate_size_t block_size) {
  visual::ColoredCharMatrix sword_sprite = visual::ColoredCharMatrix(block_size);
  common::Position position(int(block_size.x / 2), int(block_size.y / 2));
  sword_sprite.SetChar(game::visual::symbols::dagger, position, common::Color::ORANGE, common::Color::BLACK);
  return sword_sprite;
}

void Sword::Serialize(std::vector<uint8_t> &output_vector) const {
  output_vector.push_back(damage);
}

std::shared_ptr<Sword> Sword::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  int new_damage = *input_iterator++;

  auto sword = std::make_shared<Sword>(new_damage);
  return sword;
}

Sword::Sword(int damage)
    : damage(damage) {
}

int Sword::GetDamage() const {
  return damage;
}

ItemType Sword::GetItemType() {
  return ItemType::SWORD;
}
