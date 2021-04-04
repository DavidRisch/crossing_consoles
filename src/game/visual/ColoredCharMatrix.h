#ifndef CROSSING_CONSOLES_COLOREDCHARMATRIX_H
#define CROSSING_CONSOLES_COLOREDCHARMATRIX_H

#include <vector>

#include "../common/Position.h"
#include "../terminal/colors.h"
#include "ColoredChar.h"

namespace game::visual {

/**
 * \brief Matrix of `ColoredChar`s
 */
class ColoredCharMatrix {
 public:
  explicit ColoredCharMatrix(common::coordinate_size_t size);

  void AppendChar(wchar_t character, terminal::colors::Color foreground = terminal::colors::WHITE,
                  terminal::colors::Color background = terminal::colors::BLACK);

  void SetChar(wchar_t character, const common::Position& position,
               terminal::colors::Color foreground = terminal::colors::WHITE,
               terminal::colors::Color background = terminal::colors::BLACK);

  void AppendString(const std::wstring& string, terminal::colors::Color foreground = terminal::colors::WHITE,
                    terminal::colors::Color background = terminal::colors::BLACK);

  void SetString(const std::wstring& string, const common::Position& position,
                 terminal::colors::Color foreground = terminal::colors::WHITE,
                 terminal::colors::Color background = terminal::colors::BLACK);

  void InsertMatrix(const ColoredCharMatrix& matrix);

  /**
   * \brief Insert a matrix with the same width at the current position.
   */
  void AppendFullWidthMatrix(const ColoredCharMatrix& other_matrix);

  void InsertMatrix(const ColoredCharMatrix& matrix, const common::Position& position);

  const std::vector<std::vector<ColoredChar>>& GetMatrix() const;
  const common::coordinate_size_t& GetSize() const;

  bool operator==(const ColoredCharMatrix& colored_char_matrix);

  /**
   * \brief Searches for an exact match of the given string inside of a row.
   * The first match (from left to right, top to bottom) is returned.
   */
  std::optional<common::Position> Find(std::wstring needle);

 private:
  common::coordinate_size_t size;
  std::vector<std::vector<ColoredChar>> characters;

  common::Position set_current = common::Position(0, 0);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDCHARMATRIX_H
