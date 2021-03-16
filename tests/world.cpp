#include <gtest/gtest.h>

#include "../src/game/world/World.h"

using namespace game::world;
using namespace game::common;

TEST(World, AddPlayer) {
  World world(coordinate_size_t(21, 21));
  Player player("player", Position(0, 0));

  world.AddPlayer(&player);

  ASSERT_TRUE(world.players.size() == 1);

  for (auto const& i_player : world.players) {
    EXPECT_EQ(i_player, &player);
  }
}

TEST(World, AddWall) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));

  ASSERT_TRUE(world.walls.size() == 1);

  for (auto const& i_wall : world.walls) {
    EXPECT_EQ(i_wall->position, Position(10, 5));
  }
}

TEST(World, AddWallRepeated) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));
  world.AddWall(Position(10, 5));

  ASSERT_TRUE(world.walls.size() == 1);

  for (auto const& i_wall : world.walls) {
    EXPECT_EQ(i_wall->position, Position(10, 5));
  }
}

TEST(World, AddWalls) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));
  world.AddWall(Position(5, 10));

  ASSERT_TRUE(world.walls.size() == 2);

  for (auto const& i_wall : world.walls) {
    EXPECT_TRUE(i_wall->position == Position(10, 5) || i_wall->position == Position(5, 10));
  }
}

TEST(World, IsBlocked) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));

  EXPECT_TRUE(world.IsBlocked(Position(10, 5)));
}