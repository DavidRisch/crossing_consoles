#include "Renderer.h"

#include <utility>

#include "SpriteGenerator.h"

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
    , sprite_map(SpriteGenerator(block_size).InitializeMap()) {
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
