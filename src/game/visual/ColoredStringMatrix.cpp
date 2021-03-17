#include "ColoredStringMatrix.h"

#include <iostream>
#include <utility>

using namespace game::visual;
using namespace game::terminal::colors;

ColoredStringMatrix::ColoredStringMatrix(coordinate_size_t size)
    : size(std::move(size))
    , characters(size.y)
    , colors(size.y) {
  for (int y = 0; y < size.y; y++) {
    characters[y] = std::vector<wchar_t>(size.x, L' ');
    colors[y] = std::vector<std::tuple<Color, Color>>(size.x, std::make_tuple(WHITE, BLACK));
  }
}

void ColoredStringMatrix::SetChar(wchar_t character, Color foreground, Color background) {
  if (set_current.x == size.x) {
    set_current.x = 0;
    set_current.y++;
  }
  SetChar(character, set_current, foreground, background);
}

void ColoredStringMatrix::SetChar(wchar_t character, const Position& position, Color foreground, Color background) {
  if (position < size) {
    characters[position.y][position.x] = character;
    colors[position.y][position.x] = std::make_tuple(foreground, background);
    set_current = position;
    set_current.x++;
  }
}

void ColoredStringMatrix::SetString(const std::wstring& string, Color foreground, Color background) {
  for (wchar_t i_string : string) {
    SetChar(i_string, foreground, background);
  }
}

void ColoredStringMatrix::SetString(const std::wstring& string, const Position& position, Color foreground, Color background) {
  SetChar(string[0], position, foreground, background);
  for (wchar_t i_string : string.substr(1)) {
    SetChar(i_string, foreground, background);
  }
}

std::tuple<wchar_t, Color, Color> ColoredStringMatrix::GetChar() {
  if (get_current.x == size.x) {
    get_current.x = 0;
    get_current.y++;
  }
  return GetChar(get_current);
}

std::tuple<wchar_t, Color, Color> ColoredStringMatrix::GetChar(const Position& position) {
  if (position < size) {
    wchar_t character = characters[get_current.y][get_current.x];
    Color foreground = std::get<0>(colors[get_current.y][get_current.x]);
    Color background = std::get<1>(colors[get_current.y][get_current.x]);
    get_current = position;
    get_current.x++;
    return std::make_tuple(character, foreground, background);
  }
  return std::make_tuple(L'\0', WHITE, BLACK);
}
