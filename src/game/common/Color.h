#ifndef CROSSING_CONSOLES_COLOR_H
#define CROSSING_CONSOLES_COLOR_H

#include <cstdint>

#include "../networking/ISerializable.h"

namespace game::common {

/**
 * \brief A 24-bit rgb color ('true color').
 */
class Color : public networking::ISerializable {
 public:
  Color();
  Color(uint8_t red, uint8_t green, uint8_t blue);

  bool operator==(const Color& other) const;

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static Color Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  static const Color WHITE;
  static const Color BLACK;
  static const Color GREY;
  static const Color RED;
  static const Color GREEN;
  static const Color BLUE;

  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
};

}  // namespace game::common

#endif  // CROSSING_CONSOLES_COLOR_H
