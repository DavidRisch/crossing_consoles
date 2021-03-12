#ifndef CROSSING_CONSOLES_WALL_H
#define CROSSING_CONSOLES_WALL_H

#include "Position.h"

class Wall : public ISerializable {
 public:
  Position position;

  explicit Wall(Position position);

  void Serialize(std::vector<uint8_t>& into) const;

  static Wall Deserialize(std::vector<uint8_t>::iterator& from);
};

#endif  // CROSSING_CONSOLES_WALL_H
