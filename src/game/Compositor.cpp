#include "Compositor.h"

Compositor::Compositor(coordinate_size_t viewport_size, World &world, Player &player)
    : viewport_size(viewport_size)
    , world(&world)
    , player(&player) {
  renderer = new Renderer(viewport_size, block_size, world, player);
}

std::string Compositor::CompositeViewport() const {
  std::string rendered_world = renderer->RenderWorld();
  std::string out;
  out.append(std::string(viewport_size.x * block_size.x + 2, '='));
  out.append("\n> SCORE \n");
  out.append(std::string(viewport_size.x * block_size.x + 2, '='));
  out.append("\n");
  for (int y = 0; y < viewport_size.y * block_size.y; y++) {
    out.append("|");
    int rendered_world_line_length = viewport_size.x * block_size.x + 1;
    out.append(rendered_world.substr(y * rendered_world_line_length, rendered_world_line_length));
    out.insert(out.end() - 1, '|');
  }
  out.append(std::string(viewport_size.x * block_size.x + 2, '='));
  return out;
}
