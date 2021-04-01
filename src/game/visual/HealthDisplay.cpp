#include "HealthDisplay.h"

#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::visual;

game::visual::ColoredCharMatrix game::visual::HealthDisplay::Render(int health) {
  ColoredCharMatrix output(coordinate_size_t(width, 1));

  int i;
  for (i = 0; i < health; ++i) {
    output.AppendChar(symbols::black_heart_suit, terminal::colors::RED);
    output.AppendChar(L' ');  // needed because black_heart_suit is rendered extra wide
  }
  for (; i < world::Player::max_health; ++i) {
    // draw emtpy space for a heart
    output.AppendChar(symbols::box_drawings_light_right);
    output.AppendChar(symbols::box_drawings_light_left);
  }

  return output;
}
