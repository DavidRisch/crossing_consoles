#ifndef CROSSING_CONSOLES_SPRITE_H
#define CROSSING_CONSOLES_SPRITE_H

#include <string>
#include <vector>

class Sprite {
 public:
  explicit Sprite(const std::wstring& sprite);

  std::wstring GetLine(int line_number);

 private:
  std::vector<std::wstring> lines{};
};

#endif  // CROSSING_CONSOLES_SPRITE_H
