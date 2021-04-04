#ifndef CROSSING_CONSOLES_WALL_H
#define CROSSING_CONSOLES_WALL_H

#include "../common/Position.h"

namespace game::world {

/**
 * \brief Handles position information of a wall.
 */
class Wall : public networking::ISerializable {
 public:
  common::Position position;

  explicit Wall(common::Position position);

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Wall Deserialize(std::vector<uint8_t>::iterator& input_iterator);
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_WALL_H
