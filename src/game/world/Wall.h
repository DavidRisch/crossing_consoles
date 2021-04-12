#ifndef CROSSING_CONSOLES_WALL_H
#define CROSSING_CONSOLES_WALL_H

#include "../common/Position.h"
#include "block_types.h"

namespace game::world {

/**
 * \brief Handles the information of a wall.
 */
class Wall : public networking::ISerializable {
 public:
  common::Position position;
  BlockType type;

  explicit Wall(common::Position position, BlockType type = BlockType::WALL_BRICK);

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Wall Deserialize(std::vector<uint8_t>::iterator& input_iterator);
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_WALL_H
