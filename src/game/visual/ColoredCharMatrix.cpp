#include "ColoredCharMatrix.h"

#include <utility>

using namespace game;
using namespace game::common;
using namespace game::visual;
using namespace game::terminal::colors;

ColoredCharMatrix::ColoredCharMatrix(coordinate_size_t size)
    : size(std::move(size))
    , characters(size.y) {
  for (int y = 0; y < size.y; y++) {
    characters[y] = std::vector<ColoredChar>(size.x, ColoredChar(L' ', WHITE, BLACK));
  }
}

void ColoredCharMatrix::PlaceChar(wchar_t character, Color foreground, Color background) {
  if (set_current.x == size.x) {
    set_current.x = 0;
    set_current.y++;
  }
  PlaceChar(character, set_current, foreground, background);
}

void ColoredCharMatrix::PlaceChar(wchar_t character, const Position& position, Color foreground, Color background) {
  if (position < size) {
    characters[position.y][position.x] = ColoredChar(character, foreground, background);
    set_current = position;
    set_current.x++;
  }
}

void ColoredCharMatrix::PlaceString(const std::wstring& string, Color foreground, Color background) {
  for (wchar_t i_string : string) {
    PlaceChar(i_string, foreground, background);
  }
}

void ColoredCharMatrix::PlaceString(const std::wstring& string, const Position& position, Color foreground,
                                    Color background) {
  PlaceChar(string[0], position, foreground, background);
  for (wchar_t i_string : string.substr(1)) {
    PlaceChar(i_string, foreground, background);
  }
}

void ColoredCharMatrix::InsertMatrix(const ColoredCharMatrix& matrix) {
  InsertMatrix(matrix, set_current);
}

void ColoredCharMatrix::InsertMatrix(const ColoredCharMatrix& matrix, const Position& position) {
  coordinate_distance_t offset = position;
  const std::vector<std::vector<ColoredChar>>& colored_characters = matrix.GetMatrix();
  for (int y = 0; y < (int)colored_characters.size(); y++) {
    for (int x = 0; x < (int)colored_characters[y].size(); x++) {
      Position new_position = Position(x, y) + offset;
      if (new_position < size) {
        PlaceChar(colored_characters[y][x].character, new_position, colored_characters[y][x].foreground,
                  colored_characters[y][x].background);
      }
    }
  }
}

const std::vector<std::vector<ColoredChar>>& ColoredCharMatrix::GetMatrix() const {
  return characters;
}

bool ColoredCharMatrix::operator==(const ColoredCharMatrix& colored_char_matrix) {
  const std::vector<std::vector<ColoredChar>>& matrix = colored_char_matrix.GetMatrix();
  if (characters.size() == matrix.size()) {
    for (int y = 0; y < size.y; y++) {
      if (characters[y].size() != matrix[y].size()) {
        return false;
      }
      for (int x = 0; x < size.x; x++) {
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
