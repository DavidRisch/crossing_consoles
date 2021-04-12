#ifndef CROSSING_CONSOLES_COLOREDCHAR_H
#define CROSSING_CONSOLES_COLOREDCHAR_H

#include "../common/Color.h"

namespace game::visual {

/**
 * \brief Character with foreground and background colors.
 */
class ColoredChar {
 public:
  wchar_t character;
  common::Color foreground;
  common::Color background;

  ColoredChar(wchar_t character, common::Color foreground, common::Color background);

  bool operator==(const ColoredChar& colored_char) const;
  bool operator!=(const ColoredChar& colored_char) const;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDCHAR_H
