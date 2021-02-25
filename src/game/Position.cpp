#include "Position.h"

Position::Position(int x, int y) {
  this->x = x;
  this->y = y;
}

Position Position::operator+(const Position &other_position) const {
  return Position(x + other_position.x, y + other_position.y);
}

Position Position::operator-(const Position &other_position) const {
  return Position(x - other_position.x, y - other_position.y);
}

Position Position::operator%(const Position &other_position) const {
  return Position(x % other_position.x, y % other_position.y);
}

bool Position::operator==(const Position &other_position) const{
  return (x == other_position.x && y == other_position.y);
}

bool Position::operator!=(const Position &other_position) const{
  return (x != other_position.x || y != other_position.y);
}

bool Position::operator<(const Position &other_position) const{
  return (*this != other_position && x <= other_position.x && y <= other_position.y);
}

bool Position::operator<=(const Position &other_position) const{
  return (*this < other_position || *this == other_position);
}

bool Position::operator>(const Position &other_position) const{
  return (*this != other_position && x >= other_position.x && y >= other_position.y);
}

bool Position::operator>=(const Position &other_position) const{
  return (*this > other_position || *this == other_position);
}