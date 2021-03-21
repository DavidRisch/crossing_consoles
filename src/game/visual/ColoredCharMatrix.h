#ifndef CROSSING_CONSOLES_COLOREDCHARMATRIX_H
#define CROSSING_CONSOLES_COLOREDCHARMATRIX_H

#include <tuple>
#include <vector>

#include "../common/Position.h"
#include "../terminal/colors.h"
#include "ColoredChar.h"

using namespace game::common;
using namespace game::terminal::colors;

namespace game::visual {

class ColoredCharMatrix {
 public:
  explicit ColoredCharMatrix(coordinate_size_t size);

  void SetChar(wchar_t character, Color foreground = WHITE, Color background = BLACK);
  void SetChar(wchar_t character, const Position& position, Color foreground = WHITE, Color background = BLACK);
  void SetString(const std::wstring& string, Color foreground = WHITE, Color background = BLACK);
  void SetString(const std::wstring& string, const Position& position, Color foreground = WHITE,
                 Color background = BLACK);

  const std::vector<std::vector<ColoredChar>>& GetMatrix() const;

  bool operator==(const ColoredCharMatrix& colored_char_matrix);

 private:
  coordinate_size_t size;
  std::vector<std::vector<ColoredChar>> characters;

  Position set_current = Position(0, 0);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDCHARMATRIX_H
