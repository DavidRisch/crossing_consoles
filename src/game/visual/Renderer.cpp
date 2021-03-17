#include "Renderer.h"

#include <algorithm>

#include "Sprite.h"
#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::visual::symbols;

Sprite wall_sprite = Sprite(std::wstring(2, full_block) + L"\n" + std::wstring(2, full_block));
Sprite player_sprite = Sprite(L"JL\n/\\");

Renderer::Renderer(coordinate_size_t viewport_size, coordinate_size_t block_size, World& world, Player& player)
    : block_size(block_size)
    , viewport_size(viewport_size)
    , world(&world)
    , player(&player) {
}

std::wstring Renderer::RenderWorld() const {
  world->updated = false;
  player->updated = false;

  std::wstring line(viewport_size.x * block_size.x, L' ');
  line += L'\n';
  std::wstring out;
  for (int y = 0; y < viewport_size.y * block_size.y; y++) {
    out.append(line);
  }

  coordinate_size_t viewport_size_delta(viewport_size.x / 2, viewport_size.y / 2);
  Position viewport_start =
      Position(player->position.x - viewport_size_delta.x, player->position.y - viewport_size_delta.y);
  Position viewport_end =
      Position(player->position.x + viewport_size_delta.x, player->position.y + viewport_size_delta.y);

  coordinate_factor_t negative_repetition = Position(0, 0);
  coordinate_factor_t positive_repetition = Position(1, 1);

  if (viewport_start.x < 0 || viewport_start.y < 0) {
    negative_repetition = (viewport_start - world->size + Position(1, 1)) / world->size;
  }
  if (viewport_end.x >= world->size.x || viewport_end.y >= world->size.y) {
    positive_repetition = (viewport_end + world->size - Position(1, 1)) / world->size;
  }

  for (auto const& i_wall : world->walls) {
    for (int y_factor = negative_repetition.y; y_factor < positive_repetition.y; y_factor++) {
      for (int x_factor = negative_repetition.x; x_factor < positive_repetition.x; x_factor++) {
        Position position = i_wall->position + (world->size * Position(x_factor, y_factor));
        if (position >= viewport_start && position <= viewport_end) {
          Position relative_position = position - viewport_start;
          for (int y = 0; y < block_size.y; y++) {
            out.replace(((relative_position.y * block_size.y) + y) * line_length + relative_position.x * block_size.x,
                        block_size.x, wall_sprite.GetLine(y));
          }
        }
      }
    }
  }

  for (auto const& i_player : world->players) {
    if (i_player->position >= viewport_start && i_player->position <= viewport_end) {
      Position relative_position = i_player->position - viewport_start;
      for (int y = 0; y < block_size.y; y++) {
        out.replace(((relative_position.y * block_size.y) + y) * line_length + relative_position.x * block_size.x,
                    block_size.x, player_sprite.GetLine(y));
      }
    }
  }

  return out;
}
