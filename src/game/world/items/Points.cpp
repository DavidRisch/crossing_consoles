#include "Points.h"
using namespace game;
using namespace world;

Points::Points(uint8_t value)
    : value(value)
    , item_type(ItemType::POINTS) {
}

visual::ColoredCharMatrix game::world::Points::GetSprite(game::common::coordinate_size_t block_size) {
  visual::ColoredCharMatrix point_sprite = visual::ColoredCharMatrix(block_size);
  point_sprite.AppendString(L"10");
  return point_sprite;
}

void Points::Serialize(std::vector<uint8_t>& output_vector) const {
  output_vector.push_back(value);
}

std::shared_ptr<Points> Points::Deserialize(std::vector<uint8_t>::iterator& input_iterator) {
  int new_damage = *input_iterator++;

  auto points = std::make_shared<Points>(new_damage);
  return points;
}

uint8_t Points::GetValue() const {
  return value;
}

ItemType Points::GetItemType() {
  return item_type;
}
