#include <gtest/gtest.h>

#include "../src/game/world/Wall.h"
#include "../src/game/world/World.h"

using namespace game::world;
using namespace game::common;

TEST(Spawner, One) {
  coordinate_size_t world_size(23, 46);
  World world(world_size);

  auto spawn_position = world.GetSpawner().GenerateSpawnPosition();
  EXPECT_FALSE(world.IsBlocked(spawn_position));
  auto player = std::make_shared<Player>("player", spawn_position);
  world.AddPlayer(player);
  EXPECT_TRUE(world.IsBlocked(spawn_position));
}

TEST(Spawner, Many) {
  coordinate_size_t world_size(8, 6);
  World world(world_size);

  auto free_positions = world_size.x * world_size.y;
  for (int i = 0; i < free_positions; ++i) {
    auto spawn_position = world.GetSpawner().GenerateSpawnPosition();
    EXPECT_FALSE(world.IsBlocked(spawn_position));
    auto player = std::make_shared<Player>("player", spawn_position);
    world.AddPlayer(player);
    EXPECT_TRUE(world.IsBlocked(spawn_position));
  }
}

TEST(Spawner, ManyWithWalls) {
  coordinate_size_t world_size(10, 4);
  World world(world_size);

  int wall_count = 6;
  for (int i = 0; i < wall_count; ++i) {
    world.AddWall(Position(5 + i, 4));
  }

  auto free_positions = world_size.x * world_size.y - wall_count;
  for (int i = 0; i < free_positions; ++i) {
    auto spawn_position = world.GetSpawner().GenerateSpawnPosition();
    EXPECT_FALSE(world.IsBlocked(spawn_position));
    auto player = std::make_shared<Player>("player", spawn_position);
    world.AddPlayer(player);
    EXPECT_TRUE(world.IsBlocked(spawn_position));
  }
}

TEST(Spawner, NoSpaceException) {
  coordinate_size_t world_size(8, 6);
  World world(world_size);

  auto free_positions = world_size.x * world_size.y;
  for (int i = 0; i < free_positions; ++i) {
    auto spawn_position = world.GetSpawner().GenerateSpawnPosition();
    EXPECT_FALSE(world.IsBlocked(spawn_position));
    auto player = std::make_shared<Player>("player", spawn_position);
    world.AddPlayer(player);
    EXPECT_TRUE(world.IsBlocked(spawn_position));
  }

  EXPECT_THROW(world.GetSpawner().GenerateSpawnPosition(), Spawner::NoSpaceException);
}
