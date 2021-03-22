#ifndef CROSSING_CONSOLES_COLORS_H
#define CROSSING_CONSOLES_COLORS_H

namespace game::terminal::colors {

enum Color {
  BLACK =
#ifdef _WIN32
      0,
#else
      30,
#endif
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  YELLOW,
  WHITE,
  BRIGHT_BLACK
#ifdef _WIN32
  ,
#else
  = 90,
#endif
  BRIGHT_BLUE,
  BRIGHT_GREEN,
  BRIGHT_CYAN,
  BRIGHT_RED,
  BRIGHT_MAGENTA,
  BRIGHT_YELLOW,
  BRIGHT_WHITE
};
#ifndef _WIN32
const char background_color_offset = 10;
#endif

}  // namespace game::terminal::colors

#endif  // CROSSING_CONSOLES_COLORS_H
