#ifndef CROSSING_CONSOLES_COLOREDSTRING_H
#define CROSSING_CONSOLES_COLOREDSTRING_H

#include <string>

#include "../terminal/colors.h"

using namespace game::terminal::colors;

namespace game::visual {

class ColoredString {
 public:
  std::wstring string;
  Color foreground;
  Color background;

  ColoredString(std::wstring string, Color foreground, Color background);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDSTRING_H
