#ifndef CROSSING_CONSOLES_POSITION_H
#define CROSSING_CONSOLES_POSITION_H

typedef int distance_t;

class Position {
 public:
  distance_t x;
  distance_t y;

  Position(int x, int y);

  Position operator+(const Position& other_position) const;
  Position operator-(const Position& other_position) const;
  Position operator%(const Position& other_position) const;

  bool operator==(const Position& other_position) const;
  bool operator!=(const Position& other_position) const;
  bool operator<(const Position& other_position) const;
  bool operator<=(const Position& other_position) const;
  bool operator>(const Position& other_position) const;
  bool operator>=(const Position& other_position) const;
};

typedef Position object_size_t;
#endif  // CROSSING_CONSOLES_POSITION_H
