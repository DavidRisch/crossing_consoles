#include "Heart.h"

#include <cassert>
#include <memory>

#include "../../visual/symbols.h"

using namespace game;
using namespace world;

visual::ColoredCharMatrix Heart::GetSprite(common::coordinate_size_t block_size) {
  visual::ColoredCharMatrix heart_sprite = visual::ColoredCharMatrix(block_size);
  common::Position position(int(block_size.x / 2), int(block_size.y / 2));
  heart_sprite.SetChar(game::visual::symbols::black_heart_suit, position, common::Color::WHITE, common::Color::BLACK);
  return heart_sprite;
}

void Heart::Serialize(std::vector<uint8_t> &output_vector) const {
  assert(0 <= healing && healing <= 255);
  output_vector.push_back(healing);
}

std::shared_ptr<Heart> Heart::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  int new_healing = *input_iterator++;

  auto heart = std::make_shared<Heart>(new_healing);
  return heart;
}

Heart::Heart(int healing)
    : healing(healing) {
}

int Heart::GetHealing() const {
  return healing;
}

ItemType Heart::GetItemType() {
  return ItemType::HEART;
}
