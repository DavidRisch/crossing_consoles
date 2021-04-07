#include "ColoredString.h"

#include <utility>

using namespace game;
using namespace game::common;
using namespace game::visual;

ColoredString::ColoredString(std::wstring string, Color foreground, Color background)
    : string(std::move(string))
    , foreground(std::move(foreground))
    , background(std::move(background)) {
}
