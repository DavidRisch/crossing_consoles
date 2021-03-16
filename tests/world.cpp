#include "../src/game/world/World.h"

#include <gtest/gtest.h>

using namespace game::world;
using namespace game::common;

TEST(World, AddPlayer) {
  World world(coordinate_size_t(21, 21));
  auto player = std::make_shared<Player>("player", Position(0, 0));

  world.AddPlayer(player);

  ASSERT_TRUE(world.players.size() == 1);
  EXPECT_EQ(world.players.front(), player);
}

TEST(World, AddWall) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));

  ASSERT_TRUE(world.walls.size() == 1);
  EXPECT_EQ(world.walls.begin()->second.position, Position(10, 5));
}

TEST(World, AddWallRepeated) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));
  world.AddWall(Position(10, 5));

  ASSERT_TRUE(world.walls.size() == 1);
  EXPECT_EQ(world.walls.begin()->second.position, Position(10, 5));
}

TEST(World, AddWalls) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));
  world.AddWall(Position(5, 10));

  ASSERT_TRUE(world.walls.size() == 2);

  for (auto const& pair : world.walls) {
    auto wall = pair.second;
    EXPECT_TRUE(wall.position == Position(10, 5) || wall.position == Position(5, 10));
  }
}

TEST(World, IsBlocked) {
  World world(coordinate_size_t(21, 21));

  world.AddWall(Position(10, 5));

  EXPECT_TRUE(world.IsBlocked(Position(10, 5)));
}
