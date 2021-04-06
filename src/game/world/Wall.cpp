#include "Wall.h"

using namespace game;
using namespace game::common;
using namespace game::world;

Wall::Wall(Position position, BlockType type)
    : position(position)
    , type(type) {
}

void Wall::Serialize(std::vector<uint8_t>& output_vector) const {
  position.Serialize(output_vector);
  output_vector.push_back((uint8_t)type);
}

Wall Wall::Deserialize(std::vector<uint8_t>::iterator& input_iterator) {
  Position position = Position::Deserialize(input_iterator);
  BlockType type = (BlockType)*input_iterator++;
  return Wall(position, type);
}
