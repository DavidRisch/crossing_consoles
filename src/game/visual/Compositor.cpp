#include "Compositor.h"

#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::visual::symbols;

Compositor::Compositor(coordinate_size_t viewport_size, World &world, Player &player)
    : viewport_size(viewport_size)
    , world(&world)
    , player(&player) {
  renderer = std::make_unique<Renderer>(viewport_size, block_size, composited_viewport_overhang,
                                        rendered_viewport_offset, world, player);
}

ColoredStringMatrix Compositor::CompositeViewport() const {
  ColoredStringMatrix composited_viewport = renderer->RenderWorld();
  common::coordinate_size_t block_count = viewport_size * block_size;

  std::wstring frame_top_line = box_drawings_double_down_and_right +
                                std::wstring(block_count.x, box_drawings_double_horizontal) +
                                box_drawings_double_down_and_left;
  composited_viewport.SetString(frame_top_line, Position(0, 0), BRIGHT_BLACK);

  // set line containing various information about the game
  std::wstring information = L" SCORE";
  std::wstring information_line = box_drawings_double_vertical + information +
                                  std::wstring(block_count.x - information.length(), L' ') +
                                  box_drawings_double_vertical;
  composited_viewport.SetString(information_line, Position(0, 1), BRIGHT_BLACK);

  // set separator between header and rendered viewport
  std::wstring frame_separator_line = box_drawings_double_vertical_and_right +
                                      std::wstring(block_count.x, box_drawings_double_horizontal) +
                                      box_drawings_double_vertical_and_left;
  composited_viewport.SetString(frame_separator_line, Position(0, 2), BRIGHT_BLACK);

  // set frame sides
  for (int y = rendered_viewport_offset.y; y < block_count.y + composited_viewport_overhang.y; y++) {
    composited_viewport.SetChar(box_drawings_double_vertical, Position(0, y), BRIGHT_BLACK);
    composited_viewport.SetChar(box_drawings_double_vertical,
                                Position(block_count.x + composited_viewport_overhang.x - 1, y), BRIGHT_BLACK);
  }

  std::wstring frame_bottom_line = box_drawings_double_up_and_right +
                                   std::wstring(block_count.x, box_drawings_double_horizontal) +
                                   box_drawings_double_up_and_left;
  composited_viewport.SetString(frame_bottom_line, Position(0, block_count.y + composited_viewport_overhang.y - 1),
                                BRIGHT_BLACK);

  return composited_viewport;
}
