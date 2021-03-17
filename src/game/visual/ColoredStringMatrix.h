#ifndef CROSSING_CONSOLES_COLOREDSTRINGMATRIX_H
#define CROSSING_CONSOLES_COLOREDSTRINGMATRIX_H

#include <tuple>
#include <vector>

#include "../common/Position.h"
#include "../terminal/colors.h"

using namespace game::common;
using namespace game::terminal::colors;

namespace game::visual {

class ColoredStringMatrix {
 public:
  explicit ColoredStringMatrix(coordinate_size_t size);

  void SetChar(wchar_t character, Color foreground = WHITE, Color background = BLACK);
  void SetChar(wchar_t character, const Position& position, Color foreground = WHITE, Color background = BLACK);
  void SetString(const std::wstring& string, Color foreground = WHITE, Color background = BLACK);
  void SetString(const std::wstring& string, const Position& position, Color foreground = WHITE,
                 Color background = BLACK);

  std::tuple<wchar_t, Color, Color> GetChar();
  std::tuple<wchar_t, Color, Color> GetChar(const Position& position);

 private:
  coordinate_size_t size;
  std::vector<std::vector<wchar_t>> characters;
  std::vector<std::vector<std::tuple<Color, Color>>> colors;

  Position set_current = Position(0, 0);
  Position get_current = Position(0, 0);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDSTRINGMATRIX_H
