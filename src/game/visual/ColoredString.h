#ifndef CROSSING_CONSOLES_COLOREDSTRING_H
#define CROSSING_CONSOLES_COLOREDSTRING_H

#include <string>

#include "../common/Color.h"

namespace game::visual {

/**
 * \brief String with foreground and background colors.
 */
class ColoredString {
 public:
  std::wstring string;
  common::Color foreground;
  common::Color background;

  ColoredString(std::wstring string, common::Color foreground, common::Color background);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDSTRING_H
