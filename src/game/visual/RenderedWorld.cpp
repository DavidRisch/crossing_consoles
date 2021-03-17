#include "RenderedWorld.h"

#include <iostream>
#include <utility>

using namespace game::visual;
using namespace game::terminal::colors;

RenderedWorld::RenderedWorld(coordinate_size_t size)
    : size(std::move(size))
    , characters(size.y)
    , colors(size.y) {
  for (int y = 0; y < size.y; y++) {
    characters[y] = std::vector<wchar_t>(size.x, L' ');
    colors[y] = std::vector<std::tuple<Color, Color>>(size.x, std::make_tuple(WHITE, BLACK));
  }
}

void RenderedWorld::SetChar(wchar_t character, const Position& position, Color foreground, Color background) {
  if (position < size) {
    characters[position.y][position.x] = character;
    colors[position.y][position.x] = std::make_tuple(foreground, background);
  }
}

void RenderedWorld::SetString(std::wstring string, const Position& position, Color foreground, Color background) {
  std::wstring::iterator i_string = string.begin();
  for (int y = position.y; i_string != string.end(); y++) {
    for (int x = position.x; i_string != string.end(); x++) {
      if (x < size.x || y < size.y) {
        characters[y][x] = *i_string;
        colors[y][x] = std::make_tuple(foreground, background);
        i_string++;
      } else if (x > size.x) {
        x = 0;
        y++;
      }
    }
  }
}

std::tuple<wchar_t, Color, Color> RenderedWorld::GetChar() {
  if (current.x >= size.x) {
    current.x = 0;
    current.y++;
  }
  if (current.y < size.y) {
    wchar_t character = characters[current.y][current.x];
    Color foreground = std::get<0>(colors[current.y][current.x]);
    Color background = std::get<1>(colors[current.y][current.x]);
    current.x++;
    return std::make_tuple(character, foreground, background);
  }

  return std::make_tuple(L'\0', WHITE, BLACK);
}
