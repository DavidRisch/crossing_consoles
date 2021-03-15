#include <gtest/gtest.h>

#include "../src/game/common/Position.h"

using namespace game::common;

Position position_1(10, -10);
Position position_2(20, 5);
Position position_3(20, -10);

TEST(Position, Constructor) {
  Position position(1, -1);

  ASSERT_EQ(position.x, 1);
  ASSERT_EQ(position.y, -1);
}

TEST(Position, Set) {
  Position position(1, -1);
  position.Set(2, -2);

  ASSERT_EQ(position.x, 2);
  ASSERT_EQ(position.y, -2);
}

TEST(Position, EqualTo) {
  ASSERT_TRUE(position_1 == position_1);
  ASSERT_TRUE(position_2 == position_2);
  ASSERT_FALSE(position_1 == position_2);
  ASSERT_FALSE(position_2 == position_1);
}

TEST(Position, NotEqualTo) {
  ASSERT_FALSE(position_1 != position_1);
  ASSERT_FALSE(position_2 != position_2);
  ASSERT_TRUE(position_1 != position_2);
  ASSERT_TRUE(position_2 != position_1);
}

TEST(Position, LessThan) {
  ASSERT_TRUE(position_1 < position_2);
  ASSERT_TRUE(position_3 < position_2);
  ASSERT_FALSE(position_2 < position_1);
  ASSERT_FALSE(position_1 < position_1);
  ASSERT_FALSE(position_2 < position_2);
  ASSERT_FALSE(position_3 < position_1);
}

TEST(Position, LessThanEqualTo) {
  ASSERT_TRUE(position_1 <= position_2);
  ASSERT_TRUE(position_1 <= position_1);
  ASSERT_TRUE(position_2 <= position_2);
  ASSERT_FALSE(position_2 <= position_1);
}

TEST(Position, GreaterThan) {
  ASSERT_TRUE(position_2 > position_1);
  ASSERT_TRUE(position_3 > position_1);
  ASSERT_FALSE(position_1 > position_2);
  ASSERT_FALSE(position_1 > position_1);
  ASSERT_FALSE(position_2 > position_2);
  ASSERT_FALSE(position_3 > position_2);
}

TEST(Position, GreaterThanEqualTo) {
  ASSERT_TRUE(position_2 >= position_1);
  ASSERT_TRUE(position_1 >= position_1);
  ASSERT_TRUE(position_2 >= position_2);
  ASSERT_FALSE(position_1 >= position_2);
}

TEST(Position, Addition) {
  EXPECT_EQ(position_1 + position_2, Position(30, -5));
  EXPECT_EQ(position_1 + position_2, position_2 + position_1);
}

TEST(Position, Subtraction) {
  EXPECT_EQ(position_1 - position_2, Position(-10, -15));
  EXPECT_EQ(position_2 - position_1, Position(10, 15));
}

TEST(Position, Multiplication) {
  EXPECT_EQ(position_1 * position_2, Position(200, -50));
  EXPECT_EQ(position_1 * position_2, position_2 * position_1);
}

TEST(Position, Division) {
  EXPECT_EQ(position_1 / position_2, Position(0, -2));
  EXPECT_EQ(position_2 / position_1, Position(2, 0));
  EXPECT_EQ(position_1 / position_1, Position(1, 1));
  EXPECT_EQ(position_2 / position_2, Position(1, 1));
}
