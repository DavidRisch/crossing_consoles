#ifndef CROSSING_CONSOLES_POSITION_H
#define CROSSING_CONSOLES_POSITION_H

#include <cstdint>
#include <iterator>
#include <vector>

#include "../networking/ISerializable.h"

namespace game::common {

typedef int coordinate_t;

class Position : public networking::ISerializable {
 public:
  coordinate_t x;
  coordinate_t y;

  Position(int x, int y);

  void Set(int x_new, int y_new);

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Position Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  Position operator+(const Position& other_position) const;
  Position operator-(const Position& other_position) const;
  Position operator*(const Position& other_position) const;
  Position operator/(const Position& other_position) const;

  bool IsEqual(const Position& other_position) const;
  bool IsNotEqual(const Position& other_position) const;
  bool IsLess(const Position& other_position) const;
  bool IsLessOrEqual(const Position& other_position) const;
  bool IsGreater(const Position& other_position) const;
  bool IsGreaterOrEqual(const Position& other_position) const;
};

typedef Position coordinate_size_t;
typedef Position coordinate_distance_t;
typedef Position coordinate_factor_t;

}  // namespace game::common

#endif  // CROSSING_CONSOLES_POSITION_H
