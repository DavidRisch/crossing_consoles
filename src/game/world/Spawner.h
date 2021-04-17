#ifndef CROSSING_CONSOLES_SPAWNER_H
#define CROSSING_CONSOLES_SPAWNER_H

#include <random>

#include "../common/Position.h"

namespace game::world {

class World;

/**
 * \brief Used to find suitable positions to create new objects.
 */
class Spawner {
 public:
  Spawner(World *world);

  /**
   * \brief Returns a position at which a new player or item can be placed.
   */
  common::Position GenerateSpawnPosition() const;

  /**
   * \brief Thrown if no free space to spawn a new object is found.
   */
  class NoSpaceException : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
      return "Spawner found no free space to spawn a new player.";
    }
  };

 private:
  World *world;
  static std::random_device random_device;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_SPAWNER_H
