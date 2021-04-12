#include "Color.h"

using namespace game::common;

Color::Color()
    : red(0)
    , green(0)
    , blue(0) {
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue)
    : red(red)
    , green(green)
    , blue(blue) {
}

bool Color::operator==(const Color &other) const {
  return red == other.red && green == other.green && blue == other.blue;
}

void Color::Serialize(std::vector<uint8_t> &output_vector) const {
  output_vector.push_back(red);
  output_vector.push_back(green);
  output_vector.push_back(blue);
}

Color Color::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  Color color;

  color.red = *input_iterator++;
  color.green = *input_iterator++;
  color.blue = *input_iterator++;

  return color;
}

const Color Color::WHITE = Color(255, 255, 255);
const Color Color::BLACK = Color(0, 0, 0);
const Color Color::GREY = Color(128, 128, 128);
const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::BLUE = Color(0, 0, 255);
