#ifndef CROSSING_CONSOLES_IDESERIALIZEITEM_H
#define CROSSING_CONSOLES_IDESERIALIZEITEM_H

#include <cassert>
#include <memory>
#include <vector>

#include "Gun.h"
#include "Heart.h"
#include "IItem.h"
#include "Points.h"
#include "Sword.h"

namespace game::world {

class IDeserializeItem {
 public:
  static std::shared_ptr<IItem> DeserializeItem(ItemType item_type, std::vector<uint8_t>::iterator& input_iterator) {
    assert(item_type == ItemType::GUN || item_type == ItemType::SWORD || item_type == ItemType::HEART ||
           item_type == ItemType::POINTS);
    switch (item_type) {
      case ItemType::GUN:
        return Gun::Deserialize(input_iterator);
      case ItemType::SWORD:
        return Sword::Deserialize(input_iterator);
      case ItemType::HEART:
        return Heart::Deserialize(input_iterator);
      case ItemType::POINTS:
        return Points::Deserialize(input_iterator);
    }
    return std::shared_ptr<IItem>();  // to prevent compiler warning / error
  };

  static size_t DeserializeItemNumber(std::vector<uint8_t>::iterator& from) {
    return *from++;
  };
};
}  // namespace game::world
#endif  // CROSSING_CONSOLES_IDESERIALIZEITEM_H
