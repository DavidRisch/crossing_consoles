#include "../src/game/visual/Renderer.h"

#include <gtest/gtest.h>

#include "../src/game/visual/symbols.h"

using namespace game::visual;
using namespace game::common;
using namespace game::world;
using namespace game::visual::symbols;

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
      EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
    }
  }
}
