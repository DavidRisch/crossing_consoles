#ifndef CROSSING_CONSOLES_COLOREDCHARMATRIX_H
#define CROSSING_CONSOLES_COLOREDCHARMATRIX_H

#include <vector>

#include "../common/Position.h"
#include "../terminal/colors.h"
#include "ColoredChar.h"

namespace game::visual {

class ColoredCharMatrix {
 public:
  explicit ColoredCharMatrix(common::coordinate_size_t size);

  void PlaceChar(wchar_t character, terminal::colors::Color foreground = terminal::colors::WHITE,
                 terminal::colors::Color background = terminal::colors::BLACK);
  void PlaceChar(wchar_t character, const common::Position& position,
                 terminal::colors::Color foreground = terminal::colors::WHITE,
                 terminal::colors::Color background = terminal::colors::BLACK);
  void PlaceString(const std::wstring& string, terminal::colors::Color foreground = terminal::colors::WHITE,
                   terminal::colors::Color background = terminal::colors::BLACK);
  void PlaceString(const std::wstring& string, const common::Position& position,
                   terminal::colors::Color foreground = terminal::colors::WHITE,
                   terminal::colors::Color background = terminal::colors::BLACK);

  void InsertMatrix(const ColoredCharMatrix& matrix);
  void InsertMatrix(const ColoredCharMatrix& matrix, const common::Position& position);

  const std::vector<std::vector<ColoredChar>>& GetMatrix() const;
  const common::coordinate_size_t& GetSize() const;

  bool operator==(const ColoredCharMatrix& colored_char_matrix);

 private:
  common::coordinate_size_t size;
  std::vector<std::vector<ColoredChar>> characters;

  common::Position set_current = common::Position(0, 0);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDCHARMATRIX_H
