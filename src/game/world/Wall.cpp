#include "Wall.h"

using namespace game;
using namespace game::common;
using namespace game::world;

Wall::Wall(Position position)
    : position(position) {
}

void Wall::Serialize(std::vector<uint8_t>& output_vector) const {
  position.Serialize(output_vector);
}

Wall Wall::Deserialize(std::vector<uint8_t>::iterator& input_iterator) {
  return Wall(Position::Deserialize(input_iterator));
}
