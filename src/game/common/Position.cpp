#include "Position.h"

using namespace game;
using namespace game::common;

Position::Position(int x, int y) {
  this->x = x;
  this->y = y;
}

void Position::Set(int x_new, int y_new) {
  x = x_new;
  y = y_new;
}

Position Position::Deserialize(std::vector<uint8_t>::iterator &input_iterator) {
  coordinate_t x = *input_iterator++;
  coordinate_t y = *input_iterator++;
  return Position(x, y);
}

void Position::Serialize(std::vector<uint8_t> &output_vector) const {
  // TODO: use two bytes each
  output_vector.push_back(x);
  output_vector.push_back(y);
}

Position Position::operator+(const Position &other_position) const {
  return Position(x + other_position.x, y + other_position.y);
}

Position Position::operator-(const Position &other_position) const {
  return Position(x - other_position.x, y - other_position.y);
}

Position Position::operator*(const Position &other_position) const {
  return Position(x * other_position.x, y * other_position.y);
}

Position Position::operator/(const Position &other_position) const {
  return Position(x / other_position.x, y / other_position.y);
}

bool Position::operator==(const Position &other_position) const {
  return (x == other_position.x && y == other_position.y);
}

bool Position::operator!=(const Position &other_position) const {
  return (x != other_position.x || y != other_position.y);
}

bool Position::IsLess(const Position &other_position) const {
  return (x < other_position.x && y < other_position.y);
}

bool Position::IsLessOrEqual(const Position &other_position) const {
  return (x <= other_position.x && y <= other_position.y);
}

bool Position::IsGreater(const Position &other_position) const {
  return (x > other_position.x && y > other_position.y);
}

bool Position::IsGreaterOrEqual(const Position &other_position) const {
  return (x >= other_position.x && y >= other_position.y);
}