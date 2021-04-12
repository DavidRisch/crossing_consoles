#include <gtest/gtest.h>

#include "../src/game/world/HeightMap.h"

using namespace game::world;

TEST(HeightMap, Empty) {
  HeightMap height_map;

  ASSERT_EQ(height_map.GetType(-17), BlockType::EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(5), BlockType::EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(367), BlockType::EMPTY_BLOCK);
}

TEST(HeightMap, SetType) {
  HeightMap height_map;

  height_map.SetType(10, BlockType::WALL_BRICK);

  ASSERT_EQ(height_map.GetType(5), BlockType::EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(15), BlockType::WALL_BRICK);
}

TEST(HeightMap, SetTypes) {
  HeightMap height_map;

  height_map.SetType(10, BlockType::WALL_BRICK);
  height_map.SetType(20, BlockType::WALL_ROCK_FULL);
  height_map.SetType(30, BlockType::WALL_SNOW_FULL);

  ASSERT_EQ(height_map.GetType(5), BlockType::EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(15), BlockType::WALL_BRICK);
  ASSERT_EQ(height_map.GetType(25), BlockType::WALL_ROCK_FULL);
  ASSERT_EQ(height_map.GetType(35), BlockType::WALL_SNOW_FULL);
}

TEST(HeightMap, EdgeCases) {
  HeightMap height_map;

  height_map.SetType(10, BlockType::WALL_BRICK);
  height_map.SetType(20, BlockType::WALL_ROCK_FULL);
  height_map.SetType(30, BlockType::WALL_SNOW_FULL);

  ASSERT_EQ(height_map.GetType(9), BlockType::EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(10), BlockType::WALL_BRICK);
  ASSERT_EQ(height_map.GetType(20), BlockType::WALL_ROCK_FULL);
  ASSERT_EQ(height_map.GetType(30), BlockType::WALL_SNOW_FULL);
}

TEST(HeightMap, Overwrite) {
  HeightMap height_map;

  height_map.SetType(10, BlockType::WALL_BRICK);
  height_map.SetType(20, BlockType::WALL_ROCK_FULL);

  ASSERT_EQ(height_map.GetType(5), BlockType::EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(15), BlockType::WALL_BRICK);
  ASSERT_EQ(height_map.GetType(25), BlockType::WALL_ROCK_FULL);

  height_map.SetType(10, BlockType::WALL_SNOW_FULL);

  ASSERT_EQ(height_map.GetType(5), BlockType::EMPTY_BLOCK);
  ASSERT_EQ(height_map.GetType(15), BlockType::WALL_SNOW_FULL);
  ASSERT_EQ(height_map.GetType(25), BlockType::WALL_ROCK_FULL);
}
