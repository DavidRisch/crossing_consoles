#ifndef CROSSING_CONSOLES_POSITION_H
#define CROSSING_CONSOLES_POSITION_H

typedef int distance_t;

class Position {
 public:
  distance_t x;
  distance_t y;

  Position(int x, int y);
};

#endif  // CROSSING_CONSOLES_POSITION_H
