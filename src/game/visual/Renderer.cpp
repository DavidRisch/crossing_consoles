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

void Renderer::RenderElement(
    const Position& position_element, const ColoredCharMatrix& sprite, ColoredCharMatrix& rendered_world,
    const std::function<void(game::visual::ColoredCharMatrix&, const game::visual::ColoredCharMatrix*,
                             const game::common::Position&)>& change_world) const {
  for (int y_factor = negative_repetition.y; y_factor < positive_repetition.y; y_factor++) {
    for (int x_factor = negative_repetition.x; x_factor < positive_repetition.x; x_factor++) {
      // get position of element for each world repetition in world coordinates
      Position position = position_element + (world->size * Position(x_factor, y_factor));
      // check if element is within the rendered viewport
      if (position.IsGreaterOrEqual(viewport_start) && position.IsLessOrEqual(viewport_end)) {
        // get element position as rendered viewport coordinates
        Position relative_position = position - viewport_start;
        // use given lambda function to manipulate rendered_world at relative position
        change_world(rendered_world, &sprite, relative_position * block_size);
      }
    }
  }
}

ColoredCharMatrix Renderer::RenderWorld() {
  world->updated = false;
  own_player->updated = false;

  // max viewport index does not equal viewport size as the array indices starts at Position(0,0)
  auto max_viewport_index = viewport_size - coordinate_size_t(1, 1);
  ColoredCharMatrix rendered_world(viewport_size * block_size);

  // calculate delta between player and rendered viewport start/end
  coordinate_size_t viewport_size_delta(max_viewport_index.x / 2, max_viewport_index.y / 2);
  // calculate start and end of rendered viewport in world coordinates
  viewport_start =
      Position(own_player->position.x - viewport_size_delta.x, own_player->position.y - viewport_size_delta.y);
  viewport_end =
      Position(own_player->position.x + viewport_size_delta.x, own_player->position.y + viewport_size_delta.y);

  negative_repetition = Position(0, 0);
  positive_repetition = Position(1, 1);

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
    RenderElement(wall.position, sprite_map.GetSprite(wall.type), rendered_world, set_sprite);
  }

  // place projectiles
  // need to be rendered before! the player as they spawn at the same position

  for (auto const& projectile : world->GetProjectiles()) {
    RenderElement(projectile->GetPosition(), sprite_map.GetSprite(BlockType::PROJECTILE), rendered_world, set_sprite);
  }

  // place players
  for (auto const& player : world->players) {
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
    RenderElement(player->position, colored_player_sprite, rendered_world, set_sprite);
  }

  // place items
  for (auto const& pair : world->items) {
    auto item_position = pair.first;
    auto item = pair.second;

    RenderElement(item_position, item->GetSprite(block_size), rendered_world, set_sprite);
  }

  // show colored fields
  for (auto const& pair : world->colored_fields) {
    auto colored_field_position = pair.first;
    RenderElement(pair.first, ColoredCharMatrix(block_size), rendered_world, color_field_red);
  }

  return rendered_world;
}
