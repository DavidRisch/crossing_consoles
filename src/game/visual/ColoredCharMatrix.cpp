#include "ColoredCharMatrix.h"

#include <utility>

using namespace game::visual;
using namespace game::terminal::colors;

ColoredCharMatrix::ColoredCharMatrix(coordinate_size_t size)
    : size(std::move(size))
    , characters(size.y) {
  for (int y = 0; y < size.y; y++) {
    characters[y] = std::vector<ColoredChar>(size.x, ColoredChar(L' ', WHITE, BLACK));
  }
}

void ColoredCharMatrix::SetChar(wchar_t character, Color foreground, Color background) {
  if (set_current.x == size.x) {
    set_current.x = 0;
    set_current.y++;
  }
  SetChar(character, set_current, foreground, background);
}

void ColoredCharMatrix::SetChar(wchar_t character, const Position& position, Color foreground, Color background) {
  if (position < size) {
    characters[position.y][position.x] = ColoredChar(character, foreground, background);
    set_current = position;
    set_current.x++;
  }
}

void ColoredCharMatrix::SetString(const std::wstring& string, Color foreground, Color background) {
  for (wchar_t i_string : string) {
    SetChar(i_string, foreground, background);
  }
}

void ColoredCharMatrix::SetString(const std::wstring& string, const Position& position, Color foreground,
                                  Color background) {
  SetChar(string[0], position, foreground, background);
  for (wchar_t i_string : string.substr(1)) {
    SetChar(i_string, foreground, background);
  }
}

const std::vector<std::vector<ColoredChar>>& ColoredCharMatrix::GetMatrix() const {
  return characters;
}

bool ColoredCharMatrix::operator==(const ColoredCharMatrix& colored_char_matrix) {
  const std::vector<std::vector<ColoredChar>>& matrix = colored_char_matrix.GetMatrix();
  if (characters.size() == matrix.size()) {
    for (int y = 0; y < size.y; y++) {
      for (int x = 0; x < size.x; x++) {
        if (x == 0 && characters[y].size() != matrix[y].size()) {
          return false;
        }
        if (!(characters[y][x] == matrix[y][x])) {
          return false;
        }
      }
    }
    return true;
  } else {
    return false;
  }
}
