#include "Renderer.h"

#include <algorithm>
#include <utility>

#include "Sprite.h"
#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::visual::symbols;

Sprite wall_sprite = Sprite(std::wstring(2, light_shade) + L"\n" + std::wstring(2, light_shade));
Sprite player_sprite = Sprite(L"><\n/\\");

Renderer::Renderer(common::coordinate_size_t viewport_size, common::coordinate_size_t block_size,
                   common::coordinate_size_t composited_viewport_overhang,
                   common::coordinate_size_t rendered_viewport_offset, World& world, Player& player)
    : block_size(std::move(block_size))
    , viewport_size(std::move(viewport_size))
    , composited_viewport_overhang(std::move(composited_viewport_overhang))
    , rendered_viewport_offset(std::move(rendered_viewport_offset))
    , world(&world)
    , player(&player) {
}

ColoredStringMatrix Renderer::RenderWorld() const {
  world->updated = false;
  player->updated = false;

  ColoredStringMatrix rendered_world(viewport_size * block_size + composited_viewport_overhang);

  // calculate delta between player and rendered viewport start/end
  coordinate_size_t viewport_size_delta(viewport_size.x / 2, viewport_size.y / 2);
  // calculate start and end of rendered viewport in world coordinates
  Position viewport_start =
      Position(player->position.x - viewport_size_delta.x, player->position.y - viewport_size_delta.y);
  Position viewport_end =
      Position(player->position.x + viewport_size_delta.x, player->position.y + viewport_size_delta.y);

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
  for (auto const& i_wall : world->walls) {
    for (int y_factor = negative_repetition.y; y_factor < positive_repetition.y; y_factor++) {
      for (int x_factor = negative_repetition.x; x_factor < positive_repetition.x; x_factor++) {
        // get position of wall for each world repetition in world coordinates
        Position position = i_wall->position + (world->size * Position(x_factor, y_factor));
        // check if wall is within the rendered viewport
        if (position >= viewport_start && position <= viewport_end) {
          // get wall position as rendered viewport coordinates
          Position relative_position = position - viewport_start;
          // place each line of the wall sprite
          for (int y = 0; y < block_size.y; y++) {
            rendered_world.SetString(wall_sprite.GetLine(y),
                                     relative_position * block_size + Position(0, y) + rendered_viewport_offset, WHITE,
                                     RED);
          }
        }
      }
    }
  }

  // place players
  for (auto const& i_player : world->players) {
    // check if player is within the rendered viewport
    if (i_player->position >= viewport_start && i_player->position <= viewport_end) {
      // get player position as rendered viewport coordinates
      Position relative_position = i_player->position - viewport_start;
      // place each line of the player sprite
      for (int y = 0; y < block_size.y; y++) {
        rendered_world.SetString(player_sprite.GetLine(y),
                                 relative_position * block_size + Position(0, y) + rendered_viewport_offset, WHITE,
                                 BLACK);
      }
    }
  }

  return rendered_world;
}
