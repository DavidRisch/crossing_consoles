#include "../src/game/world/Player.h"

#include <gtest/gtest.h>

using namespace game::world;
using namespace game::common;

TEST(Player, Constructor) {
  Player player("player", Position(0, 0));

  ASSERT_EQ(player.name, "player");
  ASSERT_TRUE(player.position.IsEqual(Position(0, 0)));
}

TEST(Player, MoveTo) {
  Player player("player", Position(0, 0));
  player.MoveTo(Position(10, 5));

  ASSERT_TRUE(player.position.IsEqual(Position(10, 5)));
}

TEST(Player, IsAlive) {
  Player player("player", Position(0, 0));

  EXPECT_TRUE(player.IsAlive());
}
