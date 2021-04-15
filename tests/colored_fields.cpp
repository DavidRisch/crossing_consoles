#include <gtest/gtest.h>

#include "../src/game/GameLogic.h"

using namespace game::world;
using namespace game::common;
using namespace game;

TEST(ColoredFields, ReduceLifetime) {
  Position position(0, 0);
  ColoredField colored_field(position);

  uint8_t original_lifetime = colored_field.GetLifetime();
  colored_field.ReduceLifetime();

  EXPECT_EQ(colored_field.GetLifetime(), original_lifetime - 1);
}
