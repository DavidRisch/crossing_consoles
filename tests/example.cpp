#include "../src/Example.h"

#include "gtest/gtest.h"

TEST(ExampleTest, Add) {
  Example e;

  EXPECT_EQ(e.Add(1, 2), 3);
  EXPECT_EQ(e.Add(-1, 2), 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}