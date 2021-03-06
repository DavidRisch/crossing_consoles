#ifndef CROSSING_CONSOLES_COLOREDCHARMATRIX_H
#define CROSSING_CONSOLES_COLOREDCHARMATRIX_H

#include <vector>

#include "../common/Position.h"
#include "ColoredChar.h"

namespace game::visual {

/**
 * \brief Matrix of `ColoredChar`s
 */
class ColoredCharMatrix {
 public:
  explicit ColoredCharMatrix(common::coordinate_size_t size);

  void AppendChar(wchar_t character, common::Color foreground = common::Color::WHITE,
                  common::Color background = common::Color::BLACK);

  void SetChar(wchar_t character, const common::Position& position, common::Color foreground = common::Color::WHITE,
               common::Color background = common::Color::BLACK);

  void AppendString(const std::wstring& string, const common::Color& foreground = common::Color::WHITE,
                    const common::Color& background = common::Color::BLACK);

  void SetString(const std::wstring& string, const common::Position& position,
                 const common::Color& foreground = common::Color::WHITE,
                 const common::Color& background = common::Color::BLACK);

  void InsertMatrix(const ColoredCharMatrix& matrix);

  /**
   * \brief Insert a matrix with the same width at the current position.
   */
  void AppendFullWidthMatrix(const ColoredCharMatrix& other_matrix);

  void InsertMatrix(const ColoredCharMatrix& matrix, const common::coordinate_distance_t& offset);

  [[nodiscard]] const std::vector<std::vector<ColoredChar>>& GetMatrix() const;
  [[nodiscard]] const common::coordinate_size_t& GetSize() const;

  bool operator==(const ColoredCharMatrix& colored_char_matrix);

  /**
   * \brief Searches for an exact match of the given string inside of a row.
   * The first match (from left to right, top to bottom) is returned.
   */
  std::optional<common::Position> Find(std::wstring needle);

  /**
   * Changes the fore- and background color of the entire matrix.
   */
  void SetAllColors(const common::Color& foreground, const common::Color& background = common::Color::BLACK);

  /**
   * \brief Set the background color of a certain reason while keeping the foreground.
   */
  void SetBackgroundColorInRegion(common::Position& position, const common::coordinate_size_t& region_size,
                                  const common::Color& background);

 private:
  common::coordinate_size_t size;
  std::vector<std::vector<ColoredChar>> characters;

  common::Position set_current = common::Position(0, 0);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COLOREDCHARMATRIX_H
