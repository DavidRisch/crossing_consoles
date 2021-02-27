#include "Renderer.h"

#include <algorithm>

Renderer::Renderer(coordinate_size_t viewport_size, coordinate_size_t block_size, World& world, Player& player)
    : block_size(block_size)
    , viewport_size(viewport_size)
    , world(&world)
    , player(&player) {
}

std::string Renderer::RenderWorld() const {
  world->updated = false;
  player->updated = false;

  std::string line(viewport_size.x * block_size.x, ' ');
  line += "\n";
  std::string out;
  for (int y = 0; y < viewport_size.y * block_size.y; y++) {
    out.append(line);
  }

  coordinate_size_t viewport_size_delta(viewport_size.x / 2, viewport_size.y / 2);
  Position start = Position(player->position.x - viewport_size_delta.x, player->position.y - viewport_size_delta.y);
  Position end = Position(player->position.x + viewport_size_delta.x, player->position.y + viewport_size_delta.y);

  Position negative_factor = Position(0, 0);
  Position positive_factor = Position(1, 1);

  if (start.x < 0 || start.y < 0) {
    negative_factor = (start - world->size + Position(1, 1)) / world->size;
  }
  if (end.x >= world->size.x || end.y >= world->size.y) {
    positive_factor = (end + world->size - Position(1, 1)) / world->size;
  }

  for (auto const& i_wall : world->walls) {
    for (int y_factor = negative_factor.y; y_factor < positive_factor.y; y_factor++) {
      for (int x_factor = negative_factor.x; x_factor < positive_factor.x; x_factor++) {
        Position position = i_wall.position + (world->size * Position(x_factor, y_factor));
        if (position >= start && position <= end) {
          Position relative_position = position - start;
          for (int y = 0; y < block_size.y; y++) {
            out.replace(((relative_position.y * block_size.y) + y) * line_length + relative_position.x * block_size.x,
                        block_size.x, std::string(block_size.x, '#'));
          }
        }
      }
    }
  }

  for (auto const& i_player : world->players) {
    if (i_player.position >= start && i_player.position <= end) {
      Position relative_position = i_player.position - start;
      out.replace(relative_position.y * block_size.y * line_length + relative_position.x * block_size.x, block_size.x,
                  std::string(block_size.x, 'X'));
    }
  }

  Position relative_position = viewport_size_delta;
  out.replace(((relative_position.y * block_size.y) + 0) * line_length + relative_position.x * block_size.x,
              block_size.x, "JL");
  out.replace(((relative_position.y * block_size.y) + 1) * line_length + relative_position.x * block_size.x,
              block_size.x, "/\\");

  return out;
}
