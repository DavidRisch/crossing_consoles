#ifndef CROSSING_CONSOLES_WORLD_H
#define CROSSING_CONSOLES_WORLD_H

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../common/Position.h"
#include "Player.h"
#include "Spawner.h"
#include "Wall.h"

namespace game::world {

class World : public networking::ISerializable {
 public:
  common::coordinate_size_t size;
  std::list<std::shared_ptr<Player>> players{};
  std::unordered_map<common::Position, Wall, common::Position::HashFunction> walls;
  bool updated = false;

  explicit World(common::coordinate_size_t size);

  void AddPlayer(const std::shared_ptr<Player>& player);
  void AddWall(const common::Position& position);

  bool IsBlocked(const common::Position& position);

  /**
   * \brief Replace some data with newer data contained in `server_world`.
   */
  void Update(const World& server_world);

  const Spawner& GetSpawner() const;

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static World Deserialize(std::vector<uint8_t>::iterator& input_iterator);

 private:
  Spawner spawner;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_WORLD_H
