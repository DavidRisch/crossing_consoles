#include "Compositor.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "HealthDisplay.h"
#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::visual::symbols;

Compositor::Compositor(const coordinate_size_t &viewport_size, World &world, Player &player)
    : game_viewport_size(viewport_size)
    , world(&world)
    , player(&player) {
  renderer = std::make_unique<Renderer>(viewport_size, block_size, world, player);
  player_list = std::make_unique<PlayerList>(world.players);
}

ColoredCharMatrix Compositor::CompositeViewport() const {
  ColoredCharMatrix rendered_world = renderer->RenderWorld();

  // border size = number of characters used to draw the frame
  int border_size_x = 1;

  // calculate number of characters needed to display world output
  coordinate_size_t game_character_count = game_viewport_size * block_size;

  int game_output_size_x =
      game_character_count.x + border_size_x * 2;  // one character at the left and right is added for box lines

  // Create header, printed above the world output
  ColoredCharMatrix header = CompositeHeader(game_output_size_x);

  const common::coordinate_size_t &header_size = header.GetSize();

  // Create trailer, printed below the world output
  ColoredCharMatrix trailer = CompositeTrailer(game_output_size_x, player->GetItem() != nullptr);
  const common::coordinate_size_t &trailer_size = trailer.GetSize();

  // Create game output, which contains header, world output and trailer
  int game_output_size_y = game_character_count.y + header_size.y + trailer_size.y;
  ColoredCharMatrix game_output(coordinate_size_t(game_output_size_x, game_output_size_y));

  // Set header
  game_output.InsertMatrix(header, Position(0, 0));

  // Set world output
  game_output.InsertMatrix(rendered_world, Position(border_size_x, header_size.y));

  for (int y = header_size.y; y < game_character_count.y + header_size.y; y++) {
    SetBorderLines(game_output, y);
  }
  auto table_output_offset = coordinate_size_t(border_size_x + 2, header_size.y + 1);

  // Set player list
  if (show_player_list) {
    ColoredCharMatrix rendered_player_list = player_list->Render();
    game_output.InsertMatrix(rendered_player_list, table_output_offset);
  }

  if (statistics_table != nullptr && show_statistics_table) {
    ColoredCharMatrix rendered_statistics_table = statistics_table->Render();
    game_output.InsertMatrix(rendered_statistics_table, table_output_offset);
  }

  // Set respawn notice
  if (!player->IsAlive()) {
    // calculate position in viewport
    std::wstring respawn_message = L" YOU ARE DEAD! WAIT FOR RESPAWN... ";
    auto position_x = static_cast<int>(game_output.GetSize().x / 2 - respawn_message.size() / 2);
    auto position_y = static_cast<int>(game_character_count.y / 2 + header.GetSize().y);

    // generate and frame respawn message
    auto respawn_matrix = DrawFrame(ColoredCharMatrix(coordinate_size_t(static_cast<int>(respawn_message.size()), 1)));
    respawn_matrix.SetString(respawn_message, Position(1, 1));
    respawn_matrix.SetAllColors(common::Color::RED);

    game_output.InsertMatrix(respawn_matrix, Position(position_x, position_y));
  }

  // Set trailer
  game_output.InsertMatrix(trailer, Position(0, game_output_size_y - trailer_size.y));

  return game_output;
}

void Compositor::SetConnectionStatistics(
    const communication::connection_layer::ConnectionStatistics &connection_statistics) {
  statistics_table = std::make_unique<StatisticsTable>(connection_statistics);
}

ColoredCharMatrix Compositor::CompositeHeader(int viewport_width) const {
  int header_height = 6;
  int current_position_y = 0;  // indicates current line, that needs to be set
  ColoredCharMatrix header(coordinate_size_t(viewport_width, header_height));

  // Generate lines
  auto top_line = GenerateSeparatorLine(viewport_width, true, false);
  auto middle_line = GenerateSeparatorLine(viewport_width, false, false);

  header.AppendFullWidthMatrix(top_line);
  current_position_y++;

  // Set title CROSSING CONSOLES
  uint8_t title_width = title_top.size() / 2;
  auto title_position_x = viewport_width / 2 - title_width;
  assert(title_position_x >= 0);

  header.SetString(title_top, Position(title_position_x, current_position_y), common::Color::WHITE);
  SetBorderLines(header, current_position_y);
  current_position_y++;

  header.SetString(title_down, Position(title_position_x, current_position_y), common::Color::WHITE);
  SetBorderLines(header, current_position_y);
  current_position_y++;

  header.InsertMatrix(middle_line, Position(0, current_position_y));
  current_position_y++;

  // set lines containing various information about the game (Score and Health)
  std::wstring score = L" SCORE " + std::to_wstring(player->GetScore());
  header.SetString(score, Position(1, current_position_y), common::Color::WHITE);

  // show item in top bar
  std::wstring item = L"ITEM";
  header.SetString(item, Position((header.GetSize().x / 2) - 4, current_position_y), Color::WHITE);
  if (player->GetItem() != nullptr) {
    header.InsertMatrix(player->GetItem()->GetSprite(coordinate_size_t(1, 1)),
                        Position((header.GetSize().x / 2) + 2, current_position_y));
  }

  header.InsertMatrix(HealthDisplay::Render(player->health),
                      Position(header.GetSize().x - HealthDisplay::width - 2, current_position_y));

  SetBorderLines(header, current_position_y);
  current_position_y++;

  header.InsertMatrix(middle_line, Position(0, current_position_y));

  // all lines of the matrix should have been visited
  assert(current_position_y == header_height - 1);

  return header;
}

ColoredCharMatrix Compositor::CompositeTrailer(int viewport_width, bool has_item) {
  ColoredCharMatrix trailer(coordinate_size_t(viewport_width, 3));
  int position_y = 0;

  auto middle_line = GenerateSeparatorLine(viewport_width, false, false);
  trailer.InsertMatrix(middle_line, coordinate_size_t(0, position_y));
  position_y++;

  std::wstring instructions_movement = L"Movement: [WASD]  ";
  std::wstring instructions_item = L"Use Item: [SPACE]  ";
  std::wstring instruction_player = L"Player Info: [Y]  Statistics: [X]";

  int position_x =
      int(viewport_width / 2 - (instructions_movement + instructions_item + instruction_player).size() / 2);
  trailer.SetString(instructions_movement, coordinate_size_t(position_x, position_y));
  position_x += int(instructions_movement.size());

  if (has_item) {
    trailer.SetString(instructions_item, coordinate_size_t(position_x, position_y));
  } else {
    trailer.SetString(instructions_item, coordinate_size_t(position_x, position_y), common::Color::GREY);
  }

  position_x += int(instructions_item.size());
  trailer.SetString(instruction_player, coordinate_size_t(position_x, position_y));
  SetBorderLines(trailer, position_y);
  position_y++;

  auto bottom_line = GenerateSeparatorLine(viewport_width, false, true);
  trailer.InsertMatrix(bottom_line, coordinate_size_t(0, position_y));

  assert(position_y == trailer.GetSize().y - 1);

  return trailer;
}

void Compositor::SetBorderLines(ColoredCharMatrix &character_matrix, int position_y) {
  assert(character_matrix.GetSize().y > position_y);

  // Set character at the beginning of the line
  character_matrix.SetChar(box_drawings_double_vertical, Position(0, position_y), common::Color::WHITE);

  // Set character at the end of the line
  character_matrix.SetChar(box_drawings_double_vertical, Position(character_matrix.GetSize().x - 1, position_y),
                           common::Color::WHITE);
}

ColoredCharMatrix Compositor::GenerateSeparatorLine(int viewport_width, bool is_first, bool is_last) {
  assert(!is_first || !is_last);

  ColoredCharMatrix row(coordinate_size_t(viewport_width, 1));

  // Set start symbol
  if (is_first) {
    row.AppendChar(symbols::box_drawings_double_down_and_right);
  } else if (is_last) {
    row.AppendChar(symbols::box_drawings_double_up_and_right);
  } else {
    row.AppendChar(symbols::box_drawings_double_vertical_and_right);
  }

  // Draw separator line
  for (int i = 1; i < viewport_width - 1; i++) {
    row.AppendChar(symbols::box_drawings_double_horizontal);
  }

  // Set end symbol
  Position last_position(row.GetSize().x - 1, 0);
  if (is_first) {
    row.SetChar(symbols::box_drawings_double_down_and_left, last_position);
  } else if (is_last) {
    row.SetChar(symbols::box_drawings_double_up_and_left, last_position);
  } else {
    row.SetChar(symbols::box_drawings_double_vertical_and_left, last_position);
  }

  return row;
}

ColoredCharMatrix Compositor::DrawFrame(const ColoredCharMatrix &content) {
  // calculate new matrix dimensions including the new box lines
  auto result_height = content.GetSize().y + 2;
  auto result_width = content.GetSize().x + 2;

  ColoredCharMatrix result(coordinate_size_t(result_width, result_height));

  // generate top and bottom line and add side lines
  ColoredCharMatrix top_line(coordinate_size_t(0, 0));

  top_line = GenerateSeparatorLine(result_width, true, false);

  result.InsertMatrix(top_line, Position(0, 0));
  result.InsertMatrix(content, Position(1, 1));

  auto bottom_line = GenerateSeparatorLine(result_width, false, true);
  result.InsertMatrix(bottom_line, Position(0, result_height - 1));

  for (int i = 1; i < result.GetSize().y - 1; i++) {
    SetBorderLines(result, i);
  }

  return result;
}
