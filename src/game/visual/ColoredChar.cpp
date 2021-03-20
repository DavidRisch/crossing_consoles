#include "ColoredChar.h"

using namespace game::visual;
using namespace game::terminal::colors;

ColoredChar::ColoredChar(wchar_t character, Color foreground, Color background)
    : character(character)
    , foreground(foreground)
    , background(background) {
}

bool ColoredChar::operator==(const ColoredChar &colored_char) const {
  return (this->character == colored_char.character && this->foreground == colored_char.foreground &&
          this->background == colored_char.background);
}