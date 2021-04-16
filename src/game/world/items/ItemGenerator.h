#ifndef CROSSING_CONSOLES_ITEMGENERATOR_H
#define CROSSING_CONSOLES_ITEMGENERATOR_H

#include <memory>
#include <random>

#include "../Spawner.h"
#include "IItem.h"

namespace game::world {

class World;

/**
 * \brief Generates a new item of a random type at a random position.
 */
class ItemGenerator {
 public:
  explicit ItemGenerator(World *world);

  /**
   * \brief Generate a new item at a random position with a random type.
   */
  void GenerateItem();

 private:
  World *world;
  static std::random_device random_device;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_ITEMGENERATOR_H