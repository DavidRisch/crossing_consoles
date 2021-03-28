#ifndef CROSSING_CONSOLES_COLOREDSTRING_H
#define CROSSING_CONSOLES_COLOREDSTRING_H

#include <string>

#include "../terminal/colors.h"

namespace game::visual {

class ColoredString {
 public:
  std::wstring string;
  terminal::colors::Color foreground;
  terminal::colors::Color background;

  ColoredString(std::wstring string, terminal::colors::Color foreground, terminal::colors::Color background);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDSTRING_H
