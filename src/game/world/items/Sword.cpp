#include "Sword.h"

#include <memory>

#include "../../visual/ColoredCharMatrix.h"
#include "../../visual/SpriteGenerator.h"

using namespace game;
using namespace game::world;
using namespace game::common;
using namespace game::visual;

ColoredCharMatrix Sword::GetSprite(common::coordinate_size_t block_size) {
  ColoredCharMatrix sprite(block_size);
  sprite.SetChar(symbols::box_drawings_vertical_single_and_horizontal_double, Position(int(block_size.x / 2), 0),
                 Color::ORANGE);
  sprite.SetChar(symbols::box_drawings_double_horizontal, Position(int(block_size.x / 2), 1), Color::ORANGE);
  sprite.SetChar(symbols::box_drawings_double_horizontal, Position(int(block_size.x / 2), 2), Color::ORANGE);
  return sprite;
}

ColoredCharMatrix Sword::GetItemBarSprite() {
  ColoredCharMatrix sprite(coordinate_size_t(3, 1));
  sprite.AppendChar(symbols::box_drawings_vertical_single_and_horizontal_double, Color::ORANGE);
  sprite.AppendChar(symbols::box_drawings_double_horizontal, Color::ORANGE);
  sprite.AppendChar(symbols::box_drawings_double_horizontal, Color::ORANGE);
  return sprite;
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
  return ItemType::WEAPON;
}

WeaponType Sword::GetWeaponType() {
  return WeaponType::SWORD;
}

std::wstring Sword::GetName() {
  return name;
}
