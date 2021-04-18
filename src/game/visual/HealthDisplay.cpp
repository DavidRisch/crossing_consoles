#include "HealthDisplay.h"

#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::visual;

ColoredCharMatrix HealthDisplay::Render(int health) {
  ColoredCharMatrix output(coordinate_size_t(width, 1));

  int i;
  for (i = 0; i < health; ++i) {
    output.AppendChar(symbols::black_heart_suit, common::Color::RED);
    output.AppendChar(L' ');  // needed because black_heart_suit is rendered extra wide
  }
  for (; i < world::Player::max_health; ++i) {
    output.AppendChar(symbols::black_heart_suit, common::Color::GREY);
    output.AppendChar(L' ');  // needed because black_heart_suit is rendered extra wide
  }

  return output;
}
