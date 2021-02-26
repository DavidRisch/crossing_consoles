#include "Renderer.h"

#include <algorithm>
#include <iostream>

#include "Position.h"

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
  int line_length = line.size();

  if (start < Position(0, 0)) {
    coordinate_factor_t factor = (start / world->size).abs();
    start = start + world->size * factor;
    end = end + world->size * factor;
  } else if (end > world->size) {
    coordinate_factor_t factor = end / world->size;
    start = start - world->size * factor;
    end = end - world->size * factor;
  }

  for (auto const& i_wall : world->walls) {
    if (i_wall.position >= start && i_wall.position <= end) {
      Position relative_position = i_wall.position - start;
      for (int y = 0; y < block_size.y; y++) {
        out.replace(((relative_position.y * block_size.y) + y) * line_length + relative_position.x * block_size.x,
                    block_size.x, std::string(block_size.x, '#'));
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
