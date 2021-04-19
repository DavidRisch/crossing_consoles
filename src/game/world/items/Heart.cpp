#include "Heart.h"

#include <cassert>
#include <memory>

#include "../../visual/SpriteGenerator.h"

using namespace game;
using namespace world;

visual::ColoredCharMatrix Heart::GetSprite(common::coordinate_size_t block_size) {
  return visual::SpriteGenerator::GenerateItemSprite(std::wstring(game::visual::symbols::black_heart_suit, 1),
                                                     block_size, common::Color::RED);
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
