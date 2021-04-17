#include "../src/game/world/Player.h"

#include <gtest/gtest.h>

using namespace game::world;
using namespace game::common;

TEST(Player, Constructor) {
  Player player("player", Position(0, 0));

  ASSERT_EQ(player.name, "player");
  ASSERT_EQ(player.position, Position(0, 0));
}

TEST(Player, MoveTo) {
  Player player("player", Position(0, 0));
  player.MoveTo(Position(10, 5));

  ASSERT_EQ(player.position, Position(10, 5));
}

TEST(Player, IsAlive) {
  Player player("player", Position(0, 0));

  EXPECT_TRUE(player.IsAlive());
}

TEST(Player, HasWeapon) {
  Player player("player", Position(0, 0));
  ASSERT_TRUE(player.GetItem() == nullptr);
  player.SetItem(std::make_shared<Gun>(1, 20));
  ASSERT_TRUE(player.GetItem() != nullptr);
}

TEST(Player, DecreaseHealth) {
  Player player("player", Position(0, 0));
  int damage = 100;
  player.ChangeHealth(damage);
  ASSERT_FALSE(player.IsAlive());
}
