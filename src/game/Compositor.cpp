#include "Compositor.h"

Compositor::Compositor(coordinate_size_t viewport_size, World &world, Player &player)
    : viewport_size(viewport_size)
    , world(&world)
    , player(&player) {
  renderer = new Renderer(viewport_size, block_size, world, player);
}

std::wstring Compositor::CompositeViewport() const {
  std::wstring rendered_world = renderer->RenderWorld();
  std::wstring out;
  out += L'\u2554' + std::wstring(viewport_size.x * block_size.x, L'\u2550') + L"\u2557\n";
  out += L"\u2551 SCORE" + std::wstring(viewport_size.x * block_size.x - 6, L' ') + L"\u2551\n";
  out += L'\u2560' + std::wstring(viewport_size.x * block_size.x, L'\u2550') + L"\u2563\n";
  for (int y = 0; y < viewport_size.y * block_size.y; y++) {
    out += L"\u2551";
    int rendered_world_line_length = viewport_size.x * block_size.x + 1;
    out += rendered_world.substr(y * rendered_world_line_length, rendered_world_line_length);
    out.insert(out.end() - 1, L'\u2551');
  }
  out += L'\u255A' + std::wstring(viewport_size.x * block_size.x, L'\u2550') + L'\u255D';
  out += L"\n";
  return out;
}
