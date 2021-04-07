#ifndef CROSSING_CONSOLES_SPAWNER_H
#define CROSSING_CONSOLES_SPAWNER_H

#include "../common/Position.h"

namespace game::world {

class World;

/**
 * \brief Used to find suitable positions to create new `Player`s.
 */
class Spawner {
 public:
  Spawner(World *world);

  /**
   * \brief Returns a position at which a new player can be placed.
   */
  common::Position GenerateSpawnPosition() const;

  /**
   * \brief Thrown if no free space to spawn a new `Player` is found.
   */
  class NoSpaceException : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
      return "Spawner found no free space to spawn a new player.";
    }
  };

 private:
  World *world;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_SPAWNER_H
