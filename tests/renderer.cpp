#include "../src/game/visual/Renderer.h"

#include <gtest/gtest.h>

using namespace game;
using namespace game::visual;
using namespace game::common;
using namespace game::world;

TEST(Renderer, EmptyWorld) {
  coordinate_size_t world_size = coordinate_size_t(101, 101);
  coordinate_size_t viewport_size = coordinate_size_t(41, 21);
  coordinate_size_t block_size = coordinate_size_t(2, 2);
  Position player_position = Position(0, 0);
  Player player = Player("player", player_position);
  World world = World(world_size);
  Renderer renderer(viewport_size, block_size, world, player);

  ColoredCharMatrix rendered_world = renderer.RenderWorld();
  const std::vector<std::vector<ColoredChar>>& matrix = rendered_world.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      EXPECT_EQ(i_characters, ColoredChar(L' ', Color::WHITE, Color::BLACK));
    }
  }
}

TEST(Renderer, ProjectileInWorld) {
  coordinate_size_t world_size = coordinate_size_t(101, 101);
  coordinate_size_t viewport_size = coordinate_size_t(41, 21);
  coordinate_size_t block_size = coordinate_size_t(2, 2);
  Position player_position = Position(0, 0);
  Player player = Player("player", player_position);
  World world = World(world_size);
  Renderer renderer(viewport_size, block_size, world, player);

  ColoredCharMatrix rendered_world = renderer.RenderWorld();
  world.AddProjectile(std::make_shared<Projectile>(1, 1, 3, Position(3, 0), GameDefinition::Direction::NORTH));

  ColoredCharMatrix rendered_world_change = renderer.RenderWorld();

  EXPECT_FALSE(rendered_world_change == rendered_world);
}
