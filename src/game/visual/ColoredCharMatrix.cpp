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

ColoredChar ColoredCharMatrix::GetChar() {
  if (get_current.x == size.x) {
    get_current.x = 0;
    get_current.y++;
    return ColoredChar(L'\n', WHITE, BLACK);
  } else {
    return GetChar(get_current);
  }
}

ColoredChar ColoredCharMatrix::GetChar(const Position& position) {
  if (position < size) {
    ColoredChar colored_character = characters[get_current.y][get_current.x];
    get_current = position;
    get_current.x++;
    return colored_character;
  }
  return ColoredChar(L'\0', WHITE, BLACK);
}

ColoredString ColoredCharMatrix::GetString() {
  if (get_current.x == size.x) {
    get_current.x = 0;
    get_current.y++;
    return ColoredString(std::wstring(1, L'\n'), WHITE, BLACK);
  } else {
    return GetString(get_current);
  }
}

ColoredString ColoredCharMatrix::GetString(const Position& position) {
  if (position < size) {
    ColoredChar colored_character = GetChar(position);
    ColoredString colored_string(std::wstring(), colored_character.foreground, colored_character.background);
    while (colored_character.character != L'\0' && colored_character.foreground == colored_string.foreground &&
           colored_character.background == colored_string.background) {
      colored_string.string.push_back(colored_character.character);
      colored_character = GetChar();
    }

    if (get_current.x == 0) {
      get_current.x = size.x;
      get_current.y--;
    } else {
      get_current.x--;
    }

    return colored_string;
  }
  return ColoredString(std::wstring(1, L'\0'), WHITE, BLACK);
}
