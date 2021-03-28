#include "../src/game/world/World.h"

#include <gtest/gtest.h>

using namespace game::world;
using namespace game::common;

TEST(World, AddPlayer) {
  World world(coordinate_size_t(21, 21));
  Player player("player", Position(0, 0));

  world.AddPlayer(&player);

  ASSERT_TRUE(world.players.size() == 1);
  EXPECT_EQ(world.players.front(), &player);
}

TEST(World, AddWall) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));

  ASSERT_TRUE(world.walls.size() == 1);
  EXPECT_TRUE(world.walls.front()->position.IsEqual(Position(10, 5)));
}

TEST(World, AddWallRepeated) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));
  world.AddWall(Position(10, 5));

  ASSERT_TRUE(world.walls.size() == 1);
  EXPECT_TRUE(world.walls.front()->position.IsEqual(Position(10, 5)));
}

TEST(World, AddWalls) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));
  world.AddWall(Position(5, 10));

  ASSERT_TRUE(world.walls.size() == 2);

  for (auto const& i_wall : world.walls) {
    EXPECT_TRUE(i_wall->position.IsEqual(Position(10, 5)) || i_wall->position.IsEqual(Position(5, 10)));
  }
}

TEST(World, IsBlocked) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));

  EXPECT_TRUE(world.IsBlocked(Position(10, 5)));
}
