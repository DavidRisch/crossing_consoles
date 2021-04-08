#ifndef CROSSING_CONSOLES_ITEMGENERATOR_H
#define CROSSING_CONSOLES_ITEMGENERATOR_H

#include <memory>

#include "../World.h"
#include "IItem.h"

namespace game::world {

class ItemGenerator {
 public:
  explicit ItemGenerator(std::shared_ptr<World> world);

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
  std::shared_ptr<World> world;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_ITEMGENERATOR_H
