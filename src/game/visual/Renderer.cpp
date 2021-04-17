#include "Renderer.h"

#include <algorithm>
#include <iostream>
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
  ColoredCharMatrix wall_brick_sprite(block_size);
  wall_brick_sprite.AppendString(std::wstring(9, light_shade), Color::WHITE, Color::LIGHT_BROWN);

  ColoredCharMatrix wall_rock_light_sprite(block_size);
  wall_rock_light_sprite.AppendString(std::wstring(9, light_shade), Color::GREY);

  ColoredCharMatrix wall_rock_medium_sprite(block_size);
  wall_rock_medium_sprite.AppendString(std::wstring(9, medium_shade), Color::GREY);

  ColoredCharMatrix wall_rock_heavy_sprite(block_size);
  wall_rock_heavy_sprite.AppendString(std::wstring(9, dark_shade), Color::GREY);

  ColoredCharMatrix wall_rock_full_sprite(block_size);
  wall_rock_full_sprite.AppendString(std::wstring(9, full_block), Color::GREY);

  ColoredCharMatrix wall_snow_light_sprite(block_size);
  wall_snow_light_sprite.AppendString(std::wstring(9, light_shade), Color::WHITE, Color::GREY);

  ColoredCharMatrix wall_snow_medium_sprite(block_size);
  wall_snow_medium_sprite.AppendString(std::wstring(9, medium_shade), Color::WHITE, Color::GREY);

  ColoredCharMatrix wall_snow_heavy_sprite(block_size);
  wall_snow_heavy_sprite.AppendString(std::wstring(9, dark_shade), Color::WHITE, Color::GREY);

  ColoredCharMatrix wall_snow_full_sprite(block_size);
  wall_snow_full_sprite.AppendString(std::wstring(9, full_block), Color::WHITE);

  ColoredCharMatrix player_up_sprite(block_size);
  player_up_sprite.AppendChar(L' ');
  player_up_sprite.AppendChar(white_circle);
  player_up_sprite.AppendChar(L' ');
  player_up_sprite.AppendChar(box_drawings_light_up_and_right);
  player_up_sprite.AppendChar(box_drawings_light_vertical_and_horizontal);
  player_up_sprite.AppendChar(box_drawings_light_up_and_left);
  player_up_sprite.AppendString(L"/ \\");

  ColoredCharMatrix player_down_sprite(block_size);
  player_down_sprite.AppendChar(L' ');
  player_down_sprite.AppendChar(white_circle);
  player_down_sprite.AppendChar(L' ');
  player_down_sprite.AppendChar(box_drawings_light_down_and_right);
  player_down_sprite.AppendChar(box_drawings_light_vertical_and_horizontal);
  player_down_sprite.AppendChar(box_drawings_light_down_and_left);
  player_down_sprite.AppendString(L"/ \\");

  ColoredCharMatrix player_left_sprite(block_size);
  player_left_sprite.AppendChar(L' ');
  player_left_sprite.AppendChar(white_circle);
  player_left_sprite.AppendChar(L' ');
  player_left_sprite.AppendChar(box_drawings_light_horizontal);
  player_left_sprite.AppendChar(box_drawings_light_vertical_and_horizontal);
  player_left_sprite.AppendChar(L' ');
  player_left_sprite.AppendString(L"/ \\");

  ColoredCharMatrix player_right_sprite(block_size);
  player_right_sprite.AppendChar(L' ');
  player_right_sprite.AppendChar(white_circle);
  player_right_sprite.AppendChar(L' ');
  player_right_sprite.AppendChar(L' ');
  player_right_sprite.AppendChar(box_drawings_light_vertical_and_horizontal);
  player_right_sprite.AppendChar(box_drawings_light_horizontal);
  player_right_sprite.AppendString(L"/ \\");

  ColoredCharMatrix player_dead_sprite(block_size);
  player_dead_sprite.AppendChar(L' ');
  player_dead_sprite.AppendChar(L' ');
  player_dead_sprite.AppendChar(L' ');
  player_dead_sprite.AppendChar(box_drawings_light_down_and_right);
  player_dead_sprite.AppendChar(box_drawings_light_vertical_and_horizontal);
  player_dead_sprite.AppendChar(box_drawings_light_down_and_left);
  player_dead_sprite.AppendString(L"/ \\");

  ColoredCharMatrix projectile_sprite(block_size);
  projectile_sprite.AppendString(L"    ");
  projectile_sprite.AppendChar(bullet);
  projectile_sprite.AppendString(L"    ");

  sprite_map.SetSprite(BlockType::WALL_BRICK, wall_brick_sprite);
  sprite_map.SetSprite(BlockType::WALL_ROCK_LIGHT, wall_rock_light_sprite);
  sprite_map.SetSprite(BlockType::WALL_ROCK_MEDIUM, wall_rock_medium_sprite);
  sprite_map.SetSprite(BlockType::WALL_ROCK_HEAVY, wall_rock_heavy_sprite);
  sprite_map.SetSprite(BlockType::WALL_ROCK_FULL, wall_rock_full_sprite);
  sprite_map.SetSprite(BlockType::WALL_SNOW_LIGHT, wall_snow_light_sprite);
  sprite_map.SetSprite(BlockType::WALL_SNOW_MEDIUM, wall_snow_medium_sprite);
  sprite_map.SetSprite(BlockType::WALL_SNOW_HEAVY, wall_snow_heavy_sprite);
  sprite_map.SetSprite(BlockType::WALL_SNOW_FULL, wall_snow_full_sprite);
  sprite_map.SetSprite(BlockType::PLAYER_UP, player_up_sprite);
  sprite_map.SetSprite(BlockType::PLAYER_DOWN, player_down_sprite);
  sprite_map.SetSprite(BlockType::PLAYER_LEFT, player_left_sprite);
  sprite_map.SetSprite(BlockType::PLAYER_RIGHT, player_right_sprite);
  sprite_map.SetSprite(BlockType::PLAYER_DEAD, player_dead_sprite);
  sprite_map.SetSprite(BlockType::PROJECTILE, projectile_sprite);
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
    negative_repetition = (viewport_start - world->size) / world->size;
  }
  if (viewport_end.x >= world->size.x || viewport_end.y >= world->size.y) {
    positive_repetition = (viewport_end + world->size) / world->size;
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
          rendered_world.InsertMatrix(sprite_map.GetSprite(BlockType::PROJECTILE), relative_position * block_size);
        }
      }
    }
  }

  // place players
  for (auto const& player : world->players) {
    for (int y_factor = negative_repetition.y; y_factor < positive_repetition.y; y_factor++) {
      for (int x_factor = negative_repetition.x; x_factor < positive_repetition.x; x_factor++) {
        // get position of player for each world repetition in world coordinates
        Position position = player->position + (world->size * Position(x_factor, y_factor));
        // check if player is within the rendered viewport
        if (position.IsGreaterOrEqual(viewport_start) && position.IsLessOrEqual(viewport_end)) {
          // get player position as rendered viewport coordinates
          Position relative_position = position - viewport_start;

          // get player sprite depending on direction and life status
          ColoredCharMatrix colored_player_sprite(block_size);
          if (player->IsAlive()) {
            switch (player->direction) {
              case GameDefinition::NORTH:
                colored_player_sprite.InsertMatrix(sprite_map.GetSprite(BlockType::PLAYER_UP));
                break;
              case GameDefinition::SOUTH:
                colored_player_sprite.InsertMatrix(sprite_map.GetSprite(BlockType::PLAYER_DOWN));
                break;
              case GameDefinition::WEST:
                colored_player_sprite.InsertMatrix(sprite_map.GetSprite(BlockType::PLAYER_LEFT));
                break;
              case GameDefinition::EAST:
                colored_player_sprite.InsertMatrix(sprite_map.GetSprite(BlockType::PLAYER_RIGHT));
                break;
            }
          } else {
            colored_player_sprite.InsertMatrix(sprite_map.GetSprite(BlockType::PLAYER_DEAD));
          }
          // insert player sprite
          colored_player_sprite.SetAllColors(player->color);
          rendered_world.InsertMatrix(colored_player_sprite, relative_position * block_size);
        }
      }
    }
  }

  // place items
  for (auto const& pair : world->items) {
    auto item_position = pair.first;
    auto item = pair.second;

    for (int y_factor = negative_repetition.y; y_factor < positive_repetition.y; y_factor++) {
      for (int x_factor = negative_repetition.x; x_factor < positive_repetition.x; x_factor++) {
        // get position of item for each world repetition in world coordinates
        Position position = item_position + (world->size * Position(x_factor, y_factor));
        // check if item is within the rendered viewport
        if (position.IsGreaterOrEqual(viewport_start) && position.IsLessOrEqual(viewport_end)) {
          // get item position as rendered viewport coordinates
          Position relative_position = position - viewport_start;
          // insert item sprite
          ColoredCharMatrix item_sprite = item->GetSprite(block_size);
          rendered_world.InsertMatrix(item_sprite, relative_position * block_size);
        }
      }
    }
  }

  // show colored fields
  for (auto const& pair : world->colored_fields) {
    auto colored_field_position = pair.first;

    for (int y_factor = negative_repetition.y; y_factor < positive_repetition.y; y_factor++) {
      for (int x_factor = negative_repetition.x; x_factor < positive_repetition.x; x_factor++) {
        // get position of colored fields for each world repetition in world coordinates
        Position position = colored_field_position + (world->size * Position(x_factor, y_factor));
        // check if colored field is within the rendered viewport
        if (position.IsGreaterOrEqual(viewport_start) && position.IsLessOrEqual(viewport_end)) {
          // get position as rendered viewport coordinates
          Position relative_position = position - viewport_start;
          relative_position = relative_position * block_size;
          // color the background while keeping the foreground
          rendered_world.SetBackgroundColorInRegion(relative_position, block_size, Color::RED);
        }
      }
    }
  }

  return rendered_world;
}
