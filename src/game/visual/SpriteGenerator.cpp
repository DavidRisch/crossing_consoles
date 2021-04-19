#include "SpriteGenerator.h"

#include <utility>

#include "SpriteMap.h"
#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::visual::symbols;

ColoredCharMatrix SpriteGenerator::GenerateWallSprite(const wchar_t type, const common::Color& foreground_color,
                                                      const common::Color& background_color) {
  ColoredCharMatrix sprite_matrix(block_size);
  sprite_matrix.AppendString(std::wstring(9, type), foreground_color, background_color);
  return sprite_matrix;
}

ColoredCharMatrix SpriteGenerator::GenerateProjectileSprite() {
  ColoredCharMatrix projectile_sprite(block_size);
  projectile_sprite.AppendString(L"    ");
  projectile_sprite.AppendChar(bullet);
  projectile_sprite.AppendString(L"    ");
  return projectile_sprite;
}

ColoredCharMatrix SpriteGenerator::GeneratePlayerSprite(world::BlockType block_type) {
  ColoredCharMatrix player_sprite(block_size);

#ifdef _WIN32
  auto head = black_smiling_face;
#else
  auto head = white_circle;
#endif
  wchar_t body = box_drawings_light_vertical_and_horizontal;
  wchar_t arm_left = box_drawings_light_down_and_left;
  wchar_t arm_right = box_drawings_light_down_and_right;

  switch (block_type) {
    case (BlockType::PLAYER_UP): {
      arm_right = box_drawings_light_up_and_right;
      arm_left = box_drawings_light_up_and_left;
      break;
    }
    case (BlockType::PLAYER_DOWN): {
      break;
    }
    case (BlockType::PLAYER_LEFT): {
      arm_right = box_drawings_light_horizontal;
      arm_left = L' ';
      break;
    }
    case (BlockType::PLAYER_RIGHT): {
      arm_right = L' ';
      arm_left = box_drawings_light_horizontal;
      break;
    }
    case (BlockType::PLAYER_DEAD): {
      head = L' ';
      break;
    }
    default:
      throw std::runtime_error("Unexpected Block type in GeneratePlayerSprite");
  }

  player_sprite.AppendChar(L' ');
  player_sprite.AppendChar(head);
  player_sprite.AppendChar(L' ');
  player_sprite.AppendChar(arm_right);
  player_sprite.AppendChar(body);
  player_sprite.AppendChar(arm_left);
  player_sprite.AppendString(L"/ \\");
  return player_sprite;
}
SpriteMap SpriteGenerator::InitializeMap() {
  SpriteMap sprite_map(block_size);

  sprite_map.SetSprite(BlockType::EMPTY_BLOCK, ColoredCharMatrix(block_size));

  // Generate Walls
  sprite_map.SetSprite(BlockType::WALL_BRICK, GenerateWallSprite(light_shade, Color::WHITE, Color::LIGHT_BROWN));
  sprite_map.SetSprite(BlockType::WALL_ROCK_LIGHT, GenerateWallSprite(light_shade, Color::GREY));
  sprite_map.SetSprite(BlockType::WALL_ROCK_MEDIUM, GenerateWallSprite(medium_shade, Color::GREY));
  sprite_map.SetSprite(BlockType::WALL_ROCK_HEAVY, GenerateWallSprite(dark_shade, Color::GREY));
  sprite_map.SetSprite(BlockType::WALL_ROCK_FULL, GenerateWallSprite(full_block, Color::GREY));
  sprite_map.SetSprite(BlockType::WALL_SNOW_LIGHT, GenerateWallSprite(light_shade, Color::WHITE, Color::GREY));
  sprite_map.SetSprite(BlockType::WALL_SNOW_MEDIUM, GenerateWallSprite(medium_shade, Color::WHITE, Color::GREY));
  sprite_map.SetSprite(BlockType::WALL_SNOW_HEAVY, GenerateWallSprite(dark_shade, Color::WHITE, Color::GREY));
  sprite_map.SetSprite(BlockType::WALL_SNOW_FULL, GenerateWallSprite(full_block, Color::WHITE));

  // Generate PLayer
  sprite_map.SetSprite(BlockType::PLAYER_UP, GeneratePlayerSprite(world::BlockType::PLAYER_UP));
  sprite_map.SetSprite(BlockType::PLAYER_DOWN, GeneratePlayerSprite(BlockType::PLAYER_DOWN));
  sprite_map.SetSprite(BlockType::PLAYER_LEFT, GeneratePlayerSprite(BlockType::PLAYER_LEFT));
  sprite_map.SetSprite(BlockType::PLAYER_RIGHT, GeneratePlayerSprite(BlockType::PLAYER_RIGHT));
  sprite_map.SetSprite(BlockType::PLAYER_DEAD, GeneratePlayerSprite(BlockType::PLAYER_DEAD));

  // Generate Projectile
  sprite_map.SetSprite(BlockType::PROJECTILE, GenerateProjectileSprite());

  return sprite_map;
}

SpriteGenerator::SpriteGenerator(common::coordinate_size_t block_size)
    : block_size(std::move(block_size)) {
}

ColoredCharMatrix SpriteGenerator::GenerateItemSprite(const std::wstring& string,
                                                      const common::coordinate_size_t& block_size,
                                                      const Color& foreground_color, const Color& background_color) {
  ColoredCharMatrix item_sprite(block_size);
  common::Position position(int(block_size.x / 2) - int(string.size() / 2), int(block_size.y / 2));
  item_sprite.SetString(string, position, foreground_color, background_color);
  return item_sprite;
}

ColoredCharMatrix SpriteGenerator::GeneratePointSprite(int value, const coordinate_size_t& block_size,
                                                       const Color& foreground_color, const Color& background_color) {
  ColoredCharMatrix point_sprite(block_size);
  common::Position position(int(block_size.x / 2), int(block_size.y / 2));
  point_sprite.SetString(std::to_wstring(value), position, foreground_color, background_color);
  return point_sprite;
}
