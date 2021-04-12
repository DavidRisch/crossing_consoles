#include <gtest/gtest.h>

#include "../src/game/common/Position.h"
#include "../src/game/visual/SpriteMap.h"
#include "../src/game/visual/symbols.h"

using namespace game::visual;
using namespace game::visual::symbols;
using namespace game::common;
using namespace game::world;

TEST(SpriteMap, Empty) {
  coordinate_size_t block_size(3, 3);
  SpriteMap sprite_map(block_size);

  const std::vector<std::vector<ColoredChar>>& matrix = sprite_map.GetSprite(BlockType::EMPTY_BLOCK).GetMatrix();
  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      ASSERT_EQ(i_characters, ColoredChar(L' ', Color::WHITE, Color::BLACK));
    }
  }
}

TEST(SpriteMap, NonExistentType) {
  coordinate_size_t block_size(3, 3);
  SpriteMap sprite_map(block_size);

  EXPECT_THROW(sprite_map.GetSprite(BlockType::WALL_BRICK), SpriteMap::BlockTypeNotFoundException);
}

TEST(SpriteMap, ExistentType) {
  coordinate_size_t block_size(3, 3);
  SpriteMap sprite_map(block_size);

  ColoredCharMatrix wall_sprite(block_size);
  wall_sprite.AppendString(std::wstring(9, light_shade), Color::WHITE, Color::RED);
  sprite_map.SetSprite(BlockType::WALL_BRICK, wall_sprite);

  const std::vector<std::vector<ColoredChar>>& matrix = sprite_map.GetSprite(BlockType::WALL_BRICK).GetMatrix();
  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      ASSERT_EQ(i_characters, ColoredChar(light_shade, Color::WHITE, Color::RED));
    }
  }
}
