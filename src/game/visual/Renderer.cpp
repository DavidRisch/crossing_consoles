#include "Renderer.h"

#include <algorithm>
#include <utility>

#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::visual::symbols;
using namespace game::terminal::colors;

Renderer::Renderer(coordinate_size_t viewport_size, coordinate_size_t block_size, World& world, Player& own_player)
    : block_size(std::move(block_size))
    , viewport_size(std::move(viewport_size))
    , world(&world)
    , own_player(&own_player)
    , wall_sprite(ColoredCharMatrix(block_size))
    , player_sprite(ColoredCharMatrix(block_size))
    , projectile_sprite(ColoredCharMatrix(block_size)) {
  wall_sprite.AppendString(std::wstring(4, light_shade), WHITE, RED);
  player_sprite.AppendString(L"></\\");
  projectile_sprite.AppendString(L"o");
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
          rendered_world.InsertMatrix(wall_sprite, relative_position * block_size);
        }
      }
    }
  }

  // place projectiles
  // need to be rendered before! the player as they spawn at the same position
  for (auto const& i_projectile : world->GetProjectiles()) {
    // check if projectile is within the rendered viewport
    if (i_projectile->GetPosition().IsGreaterOrEqual(viewport_start) &&
        i_projectile->GetPosition().IsLessOrEqual(viewport_end)) {
      // get projectile position as rendered viewport coordinates
      Position relative_position = i_projectile->GetPosition() - viewport_start;
      // insert projectile sprite
      rendered_world.InsertMatrix(projectile_sprite, relative_position * block_size);
    }
  }

  // place players
  for (auto const& i_player : world->players) {
    // check if player is within the rendered viewport
    if (i_player->position.IsGreaterOrEqual(viewport_start) && i_player->position.IsLessOrEqual(viewport_end)) {
      // get player position as rendered viewport coordinates
      Position relative_position = i_player->position - viewport_start;
      // insert player sprite
      rendered_world.InsertMatrix(player_sprite, relative_position * block_size);
    }
  }

  return rendered_world;
}
