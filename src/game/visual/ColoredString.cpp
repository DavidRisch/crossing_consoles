#include "ColoredString.h"

#include <utility>

using namespace game::visual;
using namespace game::terminal::colors;

ColoredString::ColoredString(std::wstring string, Color foreground, Color background)
    : string(std::move(string))
    , foreground(foreground)
    , background(background) {
}
