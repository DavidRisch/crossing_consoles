#ifndef CROSSING_CONSOLES_HEIGHTMAP_H
#define CROSSING_CONSOLES_HEIGHTMAP_H

#include <map>

#include "block_types.h"

namespace game::world {

class HeightMap {
 public:
  void SetType(int height, BlockType type);
  BlockType GetType(int height);

 private:
  std::map<int, BlockType> height_types;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_HEIGHTMAP_H
