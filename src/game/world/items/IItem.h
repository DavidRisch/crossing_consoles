#ifndef CROSSING_CONSOLES_IITEM_H
#define CROSSING_CONSOLES_IITEM_H

#include "../../networking/ISerializable.h"

namespace game::world {

enum class ItemType : char {
  LONG_RANGE,
  SWORD,
  HEALING,
  POINTS,
  HIGHEST_ELEMENT = POINTS
};

class IItem : public networking::ISerializable {
 public:
  virtual void Display() = 0;
  virtual ItemType GetItemType() = 0;
};

}

#endif  // CROSSING_CONSOLES_IITEM_H
