#ifndef CROSSING_CONSOLES_COLOREDCHAR_H
#define CROSSING_CONSOLES_COLOREDCHAR_H

#include "../terminal/colors.h"

using namespace game::terminal::colors;

namespace game::visual {

class ColoredChar {
 public:
  wchar_t character;
  Color foreground;
  Color background;

  ColoredChar(wchar_t character, Color foreground, Color background);

  bool operator==(const ColoredChar& colored_char) const;
  bool operator!=(const ColoredChar& colored_char) const;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDCHAR_H
