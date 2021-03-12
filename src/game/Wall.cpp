#include "Wall.h"

Wall::Wall(Position position)
    : position(position) {
}

void Wall::Serialize(std::vector<uint8_t>& into) const {
  position.Serialize(into);
}

Wall Wall::Deserialize(std::vector<uint8_t>::iterator& from) {
  return Wall(Position::Deserialize(from));
}
