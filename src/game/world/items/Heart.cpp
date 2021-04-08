#include "Heart.h"

#include <memory>

#include "../../visual/symbols.h"

using namespace game;
using namespace world;

visual::ColoredCharMatrix Heart::GetSprite(common::coordinate_size_t block_size) {
  visual::ColoredCharMatrix colored_char_matrix = visual::ColoredCharMatrix(block_size);
  colored_char_matrix.AppendChar(game::visual::symbols::black_heart_suit);
  return colored_char_matrix;
}

void Heart::Serialize(std::vector<uint8_t> &output_vector) const {
  output_vector.push_back(healing);
}

std::shared_ptr<Heart> Heart::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  int new_healing = *input_iterator++;

  auto heart = std::make_shared<Heart>(new_healing);
  return heart;
}

Heart::Heart(int healing)
    : healing(healing)
    , item_type(ItemType::HEART) {
}

int Heart::GetHealing() const {
  return healing;
}

ItemType Heart::GetItemType() {
  return item_type;
}
