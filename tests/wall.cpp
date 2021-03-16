#include "../src/game/world/Wall.h"

#include <gtest/gtest.h>

using namespace game::world;
using namespace game::common;

TEST(Wall, Constructor) {
  Position position(10, 5);
  Wall wall(position);

  ASSERT_EQ(wall.position, position);
}
