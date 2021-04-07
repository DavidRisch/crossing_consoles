#include "ColoredChar.h"

#include <utility>

using namespace game;
using namespace game::visual;
using namespace game::common;

ColoredChar::ColoredChar(wchar_t character, Color foreground, Color background)
    : character(character)
    , foreground(std::move(foreground))
    , background(std::move(background)) {
}

bool ColoredChar::operator==(const ColoredChar &colored_char) const {
  return (character == colored_char.character && foreground == colored_char.foreground &&
          background == colored_char.background);
}

bool ColoredChar::operator!=(const ColoredChar &colored_char) const {
  return !(*this == colored_char);
}
