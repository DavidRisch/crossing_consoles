#include "Compositor.h"

#include "HealthDisplay.h"
#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::visual::symbols;

Compositor::Compositor(const coordinate_size_t &viewport_size, World &world, Player &player)
    : viewport_size(viewport_size)
    , world(&world)
    , player(&player) {
  renderer = std::make_unique<Renderer>(viewport_size, block_size, world, player);
  player_list = std::make_unique<PlayerList>(world.players);
}

ColoredCharMatrix Compositor::CompositeViewport() const {
  ColoredCharMatrix rendered_world = renderer->RenderWorld();
  ColoredCharMatrix rendered_player_list = player_list->Render();
  ColoredCharMatrix composited_viewport(viewport_size * block_size + composited_viewport_overhang);

  coordinate_size_t block_count = viewport_size * block_size;

  composited_viewport.InsertMatrix(rendered_world, rendered_viewport_offset);

  if (show_player_list) {
    common::coordinate_size_t player_list_viewport_offset =
        composited_viewport_overhang + common::coordinate_size_t(2, 1);

    composited_viewport.InsertMatrix(rendered_player_list, player_list_viewport_offset);
  }

  std::wstring frame_top_line = box_drawings_double_down_and_right +
                                std::wstring(block_count.x, box_drawings_double_horizontal) +
                                box_drawings_double_down_and_left;
  composited_viewport.SetString(frame_top_line, Position(0, 0), Color::WHITE);

  // set line containing various information about the game
  composited_viewport.SetChar(box_drawings_double_vertical, Position(0, 1), Color::WHITE);
  std::wstring information = L" SCORE " + std::to_wstring(player->GetScore());
  composited_viewport.SetString(information, Position(1, 1), Color::WHITE);
  composited_viewport.InsertMatrix(HealthDisplay::Render(player->health),
                                   Position(composited_viewport.GetSize().x - HealthDisplay::width - 2, 1));
  composited_viewport.SetChar(box_drawings_double_vertical, Position(composited_viewport.GetSize().x - 1, 1),
                              Color::WHITE);

  // set separator between header and rendered viewport
  std::wstring frame_separator_line = box_drawings_double_vertical_and_right +
                                      std::wstring(block_count.x, box_drawings_double_horizontal) +
                                      box_drawings_double_vertical_and_left;
  composited_viewport.SetString(frame_separator_line, Position(0, 2), Color::WHITE);

  // set frame sides
  for (int y = rendered_viewport_offset.y; y < block_count.y + composited_viewport_overhang.y; y++) {
    composited_viewport.SetChar(box_drawings_double_vertical, Position(0, y), Color::WHITE);
    composited_viewport.SetChar(box_drawings_double_vertical,
                                Position(block_count.x + composited_viewport_overhang.x - 1, y), Color::WHITE);
  }

  std::wstring frame_bottom_line = box_drawings_double_up_and_right +
                                   std::wstring(block_count.x, box_drawings_double_horizontal) +
                                   box_drawings_double_up_and_left;
  composited_viewport.SetString(frame_bottom_line, Position(0, block_count.y + composited_viewport_overhang.y - 1),
                                Color::WHITE);

  return composited_viewport;
}
