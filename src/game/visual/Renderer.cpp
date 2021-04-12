#include "Renderer.h"

#include <algorithm>
#include <utility>

#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::visual::symbols;

Renderer::Renderer(coordinate_size_t viewport_size, coordinate_size_t block_size, World& world, Player& own_player)
    : block_size(std::move(block_size))
    , viewport_size(std::move(viewport_size))
    , world(&world)
    , own_player(&own_player)
    , sprite_map(SpriteMap(block_size)) {
  ColoredCharMatrix wall_water_sprite(block_size);
  wall_water_sprite.AppendString(std::wstring(9, full_block), BRIGHT_BLUE);

  ColoredCharMatrix wall_brick_sprite(block_size);
  wall_brick_sprite.AppendString(std::wstring(9, light_shade), WHITE, RED);

  ColoredCharMatrix wall_rock_light_sprite(block_size);
  wall_rock_light_sprite.AppendString(std::wstring(9, light_shade), BRIGHT_BLACK);

  ColoredCharMatrix wall_rock_medium_sprite(block_size);
  wall_rock_medium_sprite.AppendString(std::wstring(9, medium_shade), BRIGHT_BLACK);

  ColoredCharMatrix wall_rock_heavy_sprite(block_size);
  wall_rock_heavy_sprite.AppendString(std::wstring(9, dark_shade), BRIGHT_BLACK);

  ColoredCharMatrix wall_rock_full_sprite(block_size);
  wall_rock_full_sprite.AppendString(std::wstring(9, full_block), BRIGHT_BLACK);

  ColoredCharMatrix wall_snow_light_sprite(block_size);
  wall_snow_light_sprite.AppendString(std::wstring(9, light_shade), WHITE, BRIGHT_BLACK);

  ColoredCharMatrix wall_snow_medium_sprite(block_size);
  wall_snow_medium_sprite.AppendString(std::wstring(9, medium_shade), WHITE, BRIGHT_BLACK);

  ColoredCharMatrix wall_snow_heavy_sprite(block_size);
  wall_snow_heavy_sprite.AppendString(std::wstring(9, dark_shade), WHITE, BRIGHT_BLACK);

  ColoredCharMatrix wall_snow_full_sprite(block_size);
  wall_snow_full_sprite.AppendString(std::wstring(9, full_block), WHITE);

  ColoredCharMatrix player_sprite(block_size);
  player_sprite.AppendChar(L' ');
  player_sprite.AppendChar(white_circle);
  player_sprite.AppendChar(L' ');
  player_sprite.AppendChar(box_drawings_light_horizontal);
  player_sprite.AppendChar(box_drawings_light_vertical_and_horizontal);
  player_sprite.AppendChar(box_drawings_light_horizontal);
  player_sprite.AppendString(L"/ \\");

  ColoredCharMatrix projectile_sprite(block_size);
  projectile_sprite.AppendString(L"    o    ");

  sprite_map.SetSprite(BlockType::WALL_WATER, wall_water_sprite);
  sprite_map.SetSprite(BlockType::WALL_BRICK, wall_brick_sprite);
  sprite_map.SetSprite(BlockType::WALL_ROCK_LIGHT, wall_rock_light_sprite);
  sprite_map.SetSprite(BlockType::WALL_ROCK_MEDIUM, wall_rock_medium_sprite);
  sprite_map.SetSprite(BlockType::WALL_ROCK_HEAVY, wall_rock_heavy_sprite);
  sprite_map.SetSprite(BlockType::WALL_ROCK_FULL, wall_rock_full_sprite);
  sprite_map.SetSprite(BlockType::WALL_SNOW_LIGHT, wall_snow_light_sprite);
  sprite_map.SetSprite(BlockType::WALL_SNOW_MEDIUM, wall_snow_medium_sprite);
  sprite_map.SetSprite(BlockType::WALL_SNOW_HEAVY, wall_snow_heavy_sprite);
  sprite_map.SetSprite(BlockType::WALL_SNOW_FULL, wall_snow_full_sprite);
  sprite_map.SetSprite(BlockType::PLAYER_BLOCK, player_sprite);
  sprite_map.SetSprite(BlockType::PROJECTILE_BLOCK, projectile_sprite);
}

ColoredCharMatrix Renderer::RenderWorld() const {
  world->updated = false;
  own_player->updated = false;

  ColoredCharMatrix rendered_world(viewport_size * block_size);

  // calculate delta between player and rendered viewport start/end
  coordinate_size_t viewport_size_delta(viewport_size.x / 2, viewport_size.y / 2);
  // calculate start and end of rendered viewport in world coordinates
  Position viewport_start =
      Position(own_player->position.x - viewport_size_delta.x, own_player->position.y - viewport_size_delta.y);
  Position viewport_end =
      Position(own_player->position.x + viewport_size_delta.x, own_player->position.y + viewport_size_delta.y);

  coordinate_factor_t negative_repetition = Position(0, 0);
  coordinate_factor_t positive_repetition = Position(1, 1);

  // calculate the repetition of the world in the rendered viewport
  if (viewport_start.x < 0 || viewport_start.y < 0) {
    negative_repetition = (viewport_start - world->size + Position(1, 1)) / world->size;
  }
  if (viewport_end.x >= world->size.x || viewport_end.y >= world->size.y) {
    positive_repetition = (viewport_end + world->size - Position(1, 1)) / world->size;
  }

  // place walls
  for (auto const& pair : world->walls) {
    auto wall = pair.second;
    for (int y_factor = negative_repetition.y; y_factor < positive_repetition.y; y_factor++) {
      for (int x_factor = negative_repetition.x; x_factor < positive_repetition.x; x_factor++) {
        // get position of wall for each world repetition in world coordinates
        Position position = wall.position + (world->size * Position(x_factor, y_factor));
        // check if wall is within the rendered viewport
        if (position.IsGreaterOrEqual(viewport_start) && position.IsLessOrEqual(viewport_end)) {
          // get wall position as rendered viewport coordinates
          Position relative_position = position - viewport_start;
          // insert wall sprite
          rendered_world.InsertMatrix(sprite_map.GetSprite(wall.type), relative_position * block_size);
        }
      }
    }
  }

  // place projectiles
  // need to be rendered before! the player as they spawn at the same position
  for (auto const& projectile : world->GetProjectiles()) {
    for (int y_factor = negative_repetition.y; y_factor < positive_repetition.y; y_factor++) {
      for (int x_factor = negative_repetition.x; x_factor < positive_repetition.x; x_factor++) {
        // get position of projectile for each world repetition in world coordinates
        Position position = projectile->GetPosition() + (world->size * Position(x_factor, y_factor));
        // check if projectile is within the rendered viewport
        if (position.IsGreaterOrEqual(viewport_start) && position.IsLessOrEqual(viewport_end)) {
          // get projectile position as rendered viewport coordinates
          Position relative_position = position - viewport_start;
          // insert projectile sprite
          rendered_world.InsertMatrix(sprite_map.GetSprite(BlockType::PROJECTILE_BLOCK),
                                      relative_position * block_size);
        }
      }
    }
  }

  // place players
  for (auto const& i_player : world->players) {
    // check if player is within the rendered viewport
    if (i_player->position.IsGreaterOrEqual(viewport_start) && i_player->position.IsLessOrEqual(viewport_end)) {
      // get player position as rendered viewport coordinates
      Position relative_position = i_player->position - viewport_start;
      // insert player sprite
      ColoredCharMatrix colored_player_sprite(sprite_map.GetSprite(BlockType::PLAYER_BLOCK));
      colored_player_sprite.SetAllColors(i_player->color);
      rendered_world.InsertMatrix(colored_player_sprite, relative_position * block_size);
    }
  }

  return rendered_world;
}
