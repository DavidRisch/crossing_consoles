#ifndef CROSSING_CONSOLES_POSITION_H
#define CROSSING_CONSOLES_POSITION_H

typedef int coordinate_t;

class Position {
 public:
  coordinate_t x;
  coordinate_t y;

  Position(int x, int y);

  void Set(int x_new, int y_new);

  Position operator+(const Position& other_position) const;
  Position operator-(const Position& other_position) const;
  Position operator*(const Position& other_position) const;
  Position operator/(const Position& other_position) const;

  bool operator==(const Position& other_position) const;
  bool operator!=(const Position& other_position) const;
  bool operator<(const Position& other_position) const;
  bool operator<=(const Position& other_position) const;
  bool operator>(const Position& other_position) const;
  bool operator>=(const Position& other_position) const;
};

typedef Position coordinate_size_t;
typedef Position coordinate_distance_t;
typedef Position coordinate_factor_t;
#endif  // CROSSING_CONSOLES_POSITION_H
