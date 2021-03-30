#include "ColoredCharMatrix.h"

#include <cassert>
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

void ColoredCharMatrix::AppendChar(wchar_t character, Color foreground, Color background) {
  if (set_current.x == size.x) {
    set_current.x = 0;
    set_current.y++;
  }
  SetChar(character, set_current, foreground, background);
}

void ColoredCharMatrix::SetChar(wchar_t character, const Position& position, Color foreground, Color background) {
  if (position.IsLess(size)) {
    characters[position.y][position.x] = ColoredChar(character, foreground, background);
    set_current = position;
    set_current.x++;
  }
}

void ColoredCharMatrix::AppendString(const std::wstring& string, Color foreground, Color background) {
  for (wchar_t i_string : string) {
    AppendChar(i_string, foreground, background);
  }
}

void ColoredCharMatrix::SetString(const std::wstring& string, const Position& position, Color foreground,
                                  Color background) {
  SetChar(string[0], position, foreground, background);
  for (wchar_t i_string : string.substr(1)) {
    AppendChar(i_string, foreground, background);
  }
}

void ColoredCharMatrix::InsertMatrix(const ColoredCharMatrix& matrix) {
  InsertMatrix(matrix, set_current);
}

void ColoredCharMatrix::AppendFullWidthMatrix(const ColoredCharMatrix& other_matrix) {
  assert(size.x == other_matrix.size.x);
  assert(set_current.x == 0);

  InsertMatrix(other_matrix, set_current);
  set_current.x = 0;
  set_current.y += other_matrix.size.y;
}

void ColoredCharMatrix::InsertMatrix(const ColoredCharMatrix& matrix, const Position& position) {
  coordinate_distance_t offset = position;
  const std::vector<std::vector<ColoredChar>>& colored_characters = matrix.GetMatrix();
  for (int y = 0; y < (int)colored_characters.size(); y++) {
    for (int x = 0; x < (int)colored_characters[y].size(); x++) {
      Position new_position = Position(x, y) + offset;
      if (new_position.IsLess(size)) {
        SetChar(colored_characters[y][x].character, new_position, colored_characters[y][x].foreground,
                colored_characters[y][x].background);
      }
    }
  }
}

const std::vector<std::vector<ColoredChar>>& ColoredCharMatrix::GetMatrix() const {
  return characters;
}

const common::coordinate_size_t& ColoredCharMatrix::GetSize() const {
  return size;
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

std::optional<common::Position> ColoredCharMatrix::Find(std::wstring needle) {
  for (int y = 0; y < (int)characters.size(); y++) {
    int match_length = 0;

    const auto& line = characters[y];
    for (int x = 0; x < (int)line.size(); x++) {
      const auto& colored_character = line[x];
      if (needle[match_length] == colored_character.character) {
        match_length++;
        if (match_length == (int)needle.size()) {
          return Position(x - match_length + 1, y);
        }
      }
    }
  }

  return {};
}
