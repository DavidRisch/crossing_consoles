#ifndef CROSSING_CONSOLES_POSITION_H
#define CROSSING_CONSOLES_POSITION_H

#include <cstdint>
#include <iterator>
#include <vector>

#include "../networking/ISerializable.h"

namespace game::common {

typedef int coordinate_t;

/**
 * \brief Represents a (x, y) coordinate.
 */
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

  bool operator==(const Position& other_position) const;
  bool operator!=(const Position& other_position) const;

  /**
   * \details Return true iff the passed position is greater in both dimensions
   */
  bool IsLess(const Position& other_position) const;

  /**
   * \details Return true iff the passed position is greater or equal in both dimensions
   */
  bool IsLessOrEqual(const Position& other_position) const;

  /**
   * \details Return true iff the passed position is less in both dimensions
   */
  bool IsGreater(const Position& other_position) const;

  /**
   * \details Return true iff the passed position is less or equal in both dimensions
   */
  bool IsGreaterOrEqual(const Position& other_position) const;

  /**
   * \brief Calculate hash of position.
   */
  struct HashFunction {
    size_t operator()(const Position& pos) const {
      return pos.x ^ (pos.y << (sizeof(size_t) / 2));
    }
  };
};

typedef Position coordinate_size_t;
typedef Position coordinate_distance_t;
typedef Position coordinate_factor_t;

}  // namespace game::common

#endif  // CROSSING_CONSOLES_POSITION_H
