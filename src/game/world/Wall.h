#ifndef CROSSING_CONSOLES_WALL_H
#define CROSSING_CONSOLES_WALL_H

#include "../common/Position.h"
#include "block_types.h"

namespace game::world {

class Wall : public networking::ISerializable {
 public:
  common::Position position;
  block_types::BlockType type;

  explicit Wall(common::Position position, block_types::BlockType type = block_types::WALL_BRICK);

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Wall Deserialize(std::vector<uint8_t>::iterator& input_iterator);
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_WALL_H
