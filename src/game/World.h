#ifndef CROSSING_CONSOLES_WORLD_H
#define CROSSING_CONSOLES_WORLD_H

#include <list>
#include <vector>

#include "Player.h"
#include "Position.h"
#include "Wall.h"

class World : public ISerializable {
 public:
  coordinate_size_t size;
  std::list<Player*> players{};
  std::list<Wall*> walls{};
  bool updated = false;

  explicit World(coordinate_size_t size);

  void AddPlayer(Player* player);
  void AddWall(const Position& position);

  bool IsBlocked(const Position& position);

  void Serialize(std::vector<uint8_t>& into) const;

  static World Deserialize(std::vector<uint8_t>::iterator& from);
};

#endif  // CROSSING_CONSOLES_WORLD_H
