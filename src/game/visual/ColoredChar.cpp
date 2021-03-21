#include "ColoredChar.h"

using namespace game::visual;
using namespace game::terminal::colors;

ColoredChar::ColoredChar(wchar_t character, Color foreground, Color background)
    : character(character)
    , foreground(foreground)
    , background(background) {
}

bool ColoredChar::operator==(const ColoredChar &colored_char) const {
  return (character == colored_char.character && foreground == colored_char.foreground &&
         background == colored_char.background);
}