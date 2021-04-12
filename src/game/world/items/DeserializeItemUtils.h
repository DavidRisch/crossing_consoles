#ifndef CROSSING_CONSOLES_DESERIALIZE_ITEM_UTILS_H
#define CROSSING_CONSOLES_DESERIALIZE_ITEM_UTILS_H

#include <cassert>
#include <memory>
#include <vector>

#include "Gun.h"
#include "Heart.h"
#include "IItem.h"
#include "Points.h"
#include "Sword.h"

namespace game::world {

class DeserializeItemUtils {
 public:
  static std::shared_ptr<IItem> DeserializeItem(ItemType item_type, std::vector<uint8_t>::iterator& input_iterator) {
    switch (item_type) {
      case ItemType::GUN:
        return Gun::Deserialize(input_iterator);
      case ItemType::SWORD:
        return Sword::Deserialize(input_iterator);
      case ItemType::HEART:
        return Heart::Deserialize(input_iterator);
      case ItemType::POINTS:
        return Points::Deserialize(input_iterator);
      default:
        throw std::runtime_error("Unknown ItemType: " + std::to_string(static_cast<int>(item_type)));
    }
  };

  static size_t DeserializeItemNumber(std::vector<uint8_t>::iterator& from) {
    return *from++;
  };
};
}  // namespace game::world
#endif  // CROSSING_CONSOLES_DESERIALIZE_ITEM_UTILS_H
