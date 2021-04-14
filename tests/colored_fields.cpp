#include <gtest/gtest.h>

#include "../src/game/world/World.h"

using namespace game::world;
using namespace game::common;
using namespace game;

TEST(ColoredFields, ReduceLifetime) {
  Position position(0, 0);
  ColoredField colored_field(position);

  uint8_t original_lifetime = colored_field.GetLifetime();
  colored_field.ReduceLifetime();

  EXPECT_EQ(colored_field.GetLifetime(), original_lifetime - 1);
}

TEST(ColoredFields, SerializeOneField) {
  Position position(0, 0);
  ColoredField colored_field(position);

  std::vector<uint8_t> output_vector;
  colored_field.Serialize(output_vector);

  auto input_iterator = output_vector.begin();
  auto deserialized_field = ColoredField::Deserialize(input_iterator);

  EXPECT_EQ(colored_field.GetLifetime(), deserialized_field.GetLifetime());
  EXPECT_EQ(colored_field.GetPosition(), position);
}

TEST(ColoredFields, SerializeWorldWithField) {
  World world(coordinate_size_t(20, 20));
  Position position(0, 0);
  ColoredField colored_field(position);
  world.AddColoredField(colored_field);
  EXPECT_TRUE(world.colored_fields.size() == 1);

  std::vector<uint8_t> output_vector;
  world.Serialize(output_vector);

  auto input_iterator = output_vector.begin();
  auto deserialized_world = World::Deserialize(input_iterator);

  EXPECT_EQ(world.colored_fields.at(position).GetPosition(),
            deserialized_world.colored_fields.at(position).GetPosition());
  EXPECT_EQ(world.colored_fields.at(position).GetLifetime(),
            deserialized_world.colored_fields.at(position).GetLifetime());
}

TEST(ColoredFields, RemoveOldFieldsFromWorld) {
  World world(coordinate_size_t(20, 20));
  Position position(0, 0);
  ColoredField colored_field(position);
  colored_field.SetLifetime(1);
  world.AddColoredField(colored_field);
  EXPECT_TRUE(world.colored_fields.size() == 1);

  world.ReduceColoredFieldLifetimes();
  EXPECT_TRUE(world.colored_fields.empty());
}
