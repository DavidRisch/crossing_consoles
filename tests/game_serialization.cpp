#include <gtest/gtest.h>

#include <cstring>

#include "../src/game/world/World.h"

using namespace game;
using namespace game::common;
using namespace game::world;

template <typename T>
bool are_objects_identical(const T &a, const T &b) {
  return (std::memcmp((void *)&a, (void *)&b, sizeof(T)) == 0);
}

template <typename T>
T serialize_and_deserialize(const T &original) {
  std::vector<uint8_t> encoded;
  original.Serialize(encoded);

  EXPECT_GT(encoded.size(), 0);

  auto iterator = encoded.begin();
  auto deserialized = T::Deserialize(iterator);
  EXPECT_EQ(iterator, encoded.end());

  return deserialized;
}

TEST(GameSerialization, Position) {
  Position original(12, 23);

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_TRUE(are_objects_identical(original, deserialized));
}

TEST(GameSerialization, Wall) {
  Wall original(Position(34, 56));

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_TRUE(are_objects_identical(original, deserialized));
}

TEST(GameSerialization, World) {
  World original(Position(34, 56));

  original.AddWall(Position(1, 2));
  original.AddWall(Position(3, 4));
  original.AddWall(Position(5, 6));

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_TRUE(are_objects_identical(original.size, deserialized.size));

  ASSERT_EQ(original.walls.size(), deserialized.walls.size());
  for (const auto &wall_original : original.walls) {
    int identical_count = 0;
    for (const auto &wall_deserialized : deserialized.walls) {
      if (are_objects_identical(wall_original->position, wall_deserialized->position)) {
        identical_count++;
      }
    }
    EXPECT_EQ(identical_count, 1);
  }
}

TEST(GameSerialization, Player) {
  Player original("ABCD", Position(34, 56));

  auto deserialized = serialize_and_deserialize(original);

  EXPECT_EQ(original.name, deserialized.name);
  EXPECT_TRUE(are_objects_identical(original.position, deserialized.position));
}
