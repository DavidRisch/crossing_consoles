#ifndef CROSSING_CONSOLES_POINTS_H
#define CROSSING_CONSOLES_POINTS_H

#include <memory>

#include "IItem.h"

namespace game::world {

class Points : public IItem {
 public:
  explicit Points(uint8_t value);

  visual::ColoredCharMatrix GetSprite(common::coordinate_size_t block_size) override;
  void Serialize(std::vector<uint8_t>& output_vector) const override;
  static std::shared_ptr<Points> Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  uint8_t GetValue() const;

  ItemType GetItemType() override;

 private:
  uint8_t value;
  ItemType item_type;
};
}  // namespace game::world

#endif  // CROSSING_CONSOLES_POINTS_H
