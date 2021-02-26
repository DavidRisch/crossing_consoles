#include "Position.h"

#include <cstdlib>

Position::Position(int x, int y) {
  this->x = x;
  this->y = y;
}

void Position::Set(int x_new, int y_new) {
  x = x_new;
  y = y_new;
}

Position Position::operator+(const Position &other_position) const {
  return Position(x + other_position.x, y + other_position.y);
}

Position Position::operator-(const Position &other_position) const {
  return Position(x - other_position.x, y - other_position.y);
}


Position Position::operator*(const Position& other_position) const {
  return Position(x * other_position.x, y * other_position.y);
}

Position Position::operator/(const Position &other_position) const {
  return Position(x / other_position.x, y / other_position.y);
}

Position Position::abs() const {
  return Position(std::abs(x), std::abs(y));
}

bool Position::operator==(const Position &other_position) const {
  return (x == other_position.x && y == other_position.y);
}

bool Position::operator!=(const Position &other_position) const {
  return (x != other_position.x || y != other_position.y);
}

bool Position::operator<(const Position &other_position) const {
  return (*this != other_position && x <= other_position.x && y <= other_position.y);
}

bool Position::operator<=(const Position &other_position) const {
  return (*this < other_position || *this == other_position);
}

bool Position::operator>(const Position &other_position) const {
  return (*this != other_position && x >= other_position.x && y >= other_position.y);
}

bool Position::operator>=(const Position &other_position) const {
  return (*this > other_position || *this == other_position);
}