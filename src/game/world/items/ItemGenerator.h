#ifndef CROSSING_CONSOLES_ITEMGENERATOR_H
#define CROSSING_CONSOLES_ITEMGENERATOR_H

#include <memory>

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

  /**
   * \brief Thrown if no free space to generate a new item is found.
   */
  class NoItemSpaceException : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
      return "ItemGenerator found no free space to add a new item.";
    }
  };

 private:
  World *world;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_ITEMGENERATOR_H