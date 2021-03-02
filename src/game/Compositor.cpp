#include "Compositor.h"

#include "symbols.h"

using namespace symbols;

Compositor::Compositor(coordinate_size_t viewport_size, World &world, Player &player)
    : viewport_size(viewport_size)
    , world(&world)
    , player(&player) {
  renderer = new Renderer(viewport_size, block_size, world, player);
}

std::wstring Compositor::CompositeViewport() const {
  std::wstring rendered_world = renderer->RenderWorld();
  std::wstring out;

  out += box_drawings_double_down_and_right +
         std::wstring(viewport_size.x * block_size.x, box_drawings_double_horizontal) +
         box_drawings_double_down_and_left + L'\n';

  out += box_drawings_double_vertical + std::wstring(L" SCORE") +
         std::wstring(viewport_size.x * block_size.x - 6, L' ') + box_drawings_double_vertical + L'\n';

  out += box_drawings_double_vertical_and_right +
         std::wstring(viewport_size.x * block_size.x, box_drawings_double_horizontal) +
         box_drawings_double_vertical_and_left + L'\n';

  for (int y = 0; y < viewport_size.y * block_size.y; y++) {
    out += box_drawings_double_vertical;
    int rendered_world_line_length = viewport_size.x * block_size.x + 1;
    out += rendered_world.substr(y * rendered_world_line_length, rendered_world_line_length);
    out.insert(out.end() - 1, box_drawings_double_vertical);
  }

  out += box_drawings_double_up_and_right +
         std::wstring(viewport_size.x * block_size.x, box_drawings_double_horizontal) +
         box_drawings_double_up_and_left + L'\n';

  return out;
}
