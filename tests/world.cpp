#include "../src/game/world/World.h"

#include <gtest/gtest.h>

using namespace game::world;
using namespace game::common;
using namespace game;

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

TEST(World, AddProjectiles) {
  // Add projectiles to world

  World world(coordinate_size_t(21, 21));
  auto projectile_list = std::list<std::shared_ptr<Projectile>>();

  auto player = std::make_shared<Player>("player", Position(0, 0));
  world.AddPlayer(player);

  auto projectile_first =
      std::make_shared<Projectile>(Projectile(10, 10, player->player_id, player->position, player->direction));
  world.AddProjectile(projectile_first);
  projectile_list.push_back(projectile_first);

  auto projectile_second =
      std::make_shared<Projectile>(Projectile(102, 14, player->player_id, player->position, player->direction));
  world.AddProjectile(projectile_second);
  projectile_list.push_back(projectile_second);

  auto projectile_third =
      std::make_shared<Projectile>(Projectile(130, 44, player->player_id, player->position, player->direction));
  world.AddProjectile(projectile_third);
  projectile_list.push_back(projectile_third);

  ASSERT_TRUE(world.GetProjectiles().size() == 3);

  for (auto const& world_projectile : world.GetProjectiles()) {
    ASSERT_EQ(world_projectile->GetShooterId(), player->player_id);
    ASSERT_TRUE(world_projectile == projectile_third || world_projectile == projectile_first ||
                world_projectile == projectile_second);
  }
}

TEST(World, RemoveProjectiles) {
  // Remove projectiles from world

  World world(coordinate_size_t(21, 21));
  auto projectile_list = std::list<std::shared_ptr<Projectile>>();

  auto player = std::make_shared<Player>("player", Position(0, 0));
  world.AddPlayer(player);

  int list_size = 4;

  for (int i = 0; i < list_size; i++) {
    auto projectile =
        std::make_shared<Projectile>(Projectile(10, 10, player->player_id, player->position, player->direction));
    world.AddProjectile(projectile);
    projectile_list.push_back(projectile);
  }

  ASSERT_TRUE(world.GetProjectiles().size() == list_size);

  world.RemoveProjectiles(projectile_list);
  ASSERT_TRUE(world.GetProjectiles().empty());
}
