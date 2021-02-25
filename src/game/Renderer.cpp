#include "Renderer.h"

#include <algorithm>

#include "Position.h"

Renderer::Renderer(int width, int height, World& world, Player& player)
    : viewport_size(Position(width, height))
    , world(&world)
    , player(&player) {

}

std::string Renderer::RenderWorld() const {
  world->updated = false;
  player->updated = false;

  std::string line(viewport_size.x * block_size.x, ' ');
  line += "\n";
  std::string out;
  for (int y = 0; y < viewport_size.y; y++) {
    out.append(line);
  }

  object_size_t viewport_size_delta(viewport_size.x / 2, viewport_size.y / 2);
  Position start =
      Position(player->position.x - viewport_size_delta.x, player->position.y - viewport_size_delta.y) % world->size;
  Position end =
      Position(player->position.x + viewport_size_delta.x, player->position.y + viewport_size_delta.y) % world->size;

  for (auto const& i_wall : world->walls) {
    if (i_wall.position >= start && i_wall.position <= end) {
      Position relative_position = i_wall.position - start;
      out.replace(relative_position.y * line.size() + relative_position.x * block_size.x, 2, "##");
    }
  }

  for (auto const& i_player : world->players) {
    if (i_player.position >= start && i_player.position <= end) {
      Position relative_position = i_player.position - start;
      out.replace(relative_position.y * line.size() + relative_position.x * block_size.x, 2, "X-");
    }
  }

  Position relative_position = viewport_size_delta;
  out.replace(relative_position.y * line.size() + relative_position.x * block_size.x, 2, "X-");

  return out;
}
