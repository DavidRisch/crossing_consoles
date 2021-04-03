#include <gtest/gtest.h>

#include "../src/game/world/HeightMap.h"

using namespace game::world;
using namespace game::world::block_types;

TEST(HeightMap, Empty) {
  HeightMap height_map;

  ASSERT_EQ(height_map.GetType(-17), EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(5), EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(367), EMPTY_BLOCK);
}

TEST(HeightMap, SetType) {
  HeightMap height_map;

  height_map.SetType(10, WALL_BRICK);

  ASSERT_EQ(height_map.GetType(5), EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(15), WALL_BRICK);
}

TEST(HeightMap, SetTypes) {
  HeightMap height_map;

  height_map.SetType(10, WALL_BRICK);
  height_map.SetType(20, WALL_ROCK);
  height_map.SetType(30, WALL_SNOW);

  ASSERT_EQ(height_map.GetType(5), EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(15), WALL_BRICK);
  ASSERT_EQ(height_map.GetType(25), WALL_ROCK);
  ASSERT_EQ(height_map.GetType(35), WALL_SNOW);
}

TEST(HeightMap, EdgeCases) {
  HeightMap height_map;

  height_map.SetType(10, WALL_BRICK);
  height_map.SetType(20, WALL_ROCK);
  height_map.SetType(30, WALL_SNOW);

  ASSERT_EQ(height_map.GetType(9), EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(10), WALL_BRICK);
  ASSERT_EQ(height_map.GetType(20), WALL_ROCK);
  ASSERT_EQ(height_map.GetType(30), WALL_SNOW);
}

TEST(HeightMap, Overwrite) {
  HeightMap height_map;

  height_map.SetType(10, WALL_BRICK);
  height_map.SetType(20, WALL_ROCK);

  ASSERT_EQ(height_map.GetType(5), EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(15), WALL_BRICK);
  ASSERT_EQ(height_map.GetType(25), WALL_ROCK);

  height_map.SetType(10, WALL_WATER);

  ASSERT_EQ(height_map.GetType(5), EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(15), WALL_WATER);
  ASSERT_EQ(height_map.GetType(25), WALL_ROCK);
}