#ifndef CROSSING_CONSOLES_COLORS_H
#define CROSSING_CONSOLES_COLORS_H

namespace game::terminal::colors {

enum Color {
  BLACK = 30,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
  BRIGHT_BLACK = 90,
  BRIGHT_RED,
  BRIGHT_GREEN,
  BRIGHT_YELLOW,
  BRIGHT_BLUE,
  BRIGHT_MAGENTA,
  BRIGHT_CYAN,
  BRIGHT_WHITE
};
const char background_color_offset = 10;

}  // namespace game::terminal::colors

#endif  // CROSSING_CONSOLES_COLORS_H
