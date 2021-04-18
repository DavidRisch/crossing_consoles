#include "Sword.h"

#include <memory>

#include "../../visual/SpriteGenerator.h"

using namespace game;
using namespace world;

visual::ColoredCharMatrix Sword::GetSprite(common::coordinate_size_t block_size) {
  return visual::SpriteGenerator::GenerateItemSprite(visual::symbols::dagger, block_size, common::Color::ORANGE);
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
