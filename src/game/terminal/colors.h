#ifndef CROSSING_CONSOLES_COLORS_H
#define CROSSING_CONSOLES_COLORS_H

namespace game::terminal::colors {
#ifdef _WIN32
enum Color {
  BLACK = 0,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  YELLOW,
  WHITE,
  BRIGHT_BLACK,
  BRIGHT_BLUE,
  BRIGHT_GREEN,
  BRIGHT_CYAN,
  BRIGHT_RED,
  BRIGHT_MAGENTA,
  BRIGHT_YELLOW,
  BRIGHT_WHITE
};
#else
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
#endif
}  // namespace game::terminal::colors

#endif  // CROSSING_CONSOLES_COLORS_H
