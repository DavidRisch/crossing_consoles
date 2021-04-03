#include "Wall.h"

using namespace game;
using namespace game::common;
using namespace game::world;

Wall::Wall(Position position, block_types::BlockType type)
    : position(position)
    , type(type) {
}

void Wall::Serialize(std::vector<uint8_t>& output_vector) const {
  position.Serialize(output_vector);
}

Wall Wall::Deserialize(std::vector<uint8_t>::iterator& input_iterator) {
  return Wall(Position::Deserialize(input_iterator));
}
