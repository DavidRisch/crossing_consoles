#ifndef CROSSING_CONSOLES_RENDEREDWORLD_H
#define CROSSING_CONSOLES_RENDEREDWORLD_H

#include <tuple>
#include <vector>

#include "../common/Position.h"
#include "../terminal/colors.h"

using namespace game::common;
using namespace game::terminal::colors;

namespace game::visual {

class RenderedWorld {
 public:
  RenderedWorld(coordinate_size_t size);

  void SetChar(wchar_t character, const Position& position, Color foreground = WHITE, Color background = BLACK);
  void SetString(std::wstring string, const Position& position, Color foreground = WHITE, Color background = BLACK);

  std::tuple<wchar_t, Color, Color> GetChar();

 private:
  coordinate_size_t size;
  std::vector<std::vector<wchar_t>> characters;
  std::vector<std::vector<std::tuple<Color, Color>>> colors;

  Position current = Position(0, 0);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_RENDEREDWORLD_H
