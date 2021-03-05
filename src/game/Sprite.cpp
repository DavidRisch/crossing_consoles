#include "Sprite.h"

#include <iostream>
#include <sstream>
#include <utility>

Sprite::Sprite(const std::wstring& sprite) {
  std::wstringstream ss(sprite);
  std::wstring value;
  while (ss >> value) {
    lines.push_back(value);

    if (ss.peek() == L'\n')
      ss.ignore();
  }
}

std::wstring Sprite::GetLine(int line_number) {
  return lines[line_number];
}
