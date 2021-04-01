#include "PlayerList.h"

#include <cassert>
#include <codecvt>
#include <iomanip>
#include <locale>
#include <numeric>
#include <sstream>
#include <utility>

#include "HealthDisplay.h"
#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::visual;

const std::list<PlayerList::Column> PlayerList::columns{
    PlayerList::Column(15, "name",
                       [](const Row &row, game::visual::ColoredCharMatrix &field) {
                         std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

                         field.AppendString(converter.from_bytes(row.player_name));
                       }),
    PlayerList::Column(HealthDisplay::width, "health",
                       [](const Row &row, game::visual::ColoredCharMatrix &field) {
                         field = HealthDisplay::Render(row.player_health);
                       }),
    PlayerList::Column(11, "packet loss",
                       [](const Row &row, game::visual::ColoredCharMatrix &field) {
                         std::wstringstream stream;
                         stream << std::fixed << std::setprecision(1) << row.packet_loss_percentage << L"%";

                         field.SetString(stream.str(), Position(0, 0));
                       }),
    PlayerList::Column(10, "ping",
                       [](const Row &row, game::visual::ColoredCharMatrix &field) {
                         std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

                         if (row.ping.has_value()) {
                           auto ping_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(*row.ping).count();
                           field.AppendString(converter.from_bytes(std::to_string(ping_in_ms) + " ms"));
                         }
                       })

};

PlayerList::PlayerList(world::player_ptr_list_t &players)
    : players(players) {
}

int PlayerList::CalculateTotalWidth() {
  int all_column_widths = std::accumulate(
      columns.begin(), columns.end(), 0, [](int current, const Column &column) { return current + column.GetWidth(); });
  int line_count = static_cast<int>(columns.size()) + 1;
  return all_column_widths + line_count;
}

game::visual::ColoredCharMatrix PlayerList::Render() const {
  const int divider_height = 1;
  const int header_height = 1;
  const int row_count = divider_height + header_height + divider_height + (int)players.size() + divider_height;

  ColoredCharMatrix table(coordinate_size_t(CalculateTotalWidth(), row_count));

  table.AppendFullWidthMatrix(MakeDivider(true, false));
  table.AppendFullWidthMatrix(MakeHeader());
  table.AppendFullWidthMatrix(MakeDivider(false, false));

  std::list<Row> rows;
  for (const auto &player : players) {
    rows.emplace_back(player->name, player->health, player->packet_loss_percentage, player->ping);
  }

  for (const auto &row : rows) {
    auto rendered_row = row.RenderRow(*this);

    table.AppendFullWidthMatrix(rendered_row);
  }

  table.AppendFullWidthMatrix(MakeDivider(false, true));

  return table;
}

game::visual::ColoredCharMatrix PlayerList::MakeHeader() {
  ColoredCharMatrix row(coordinate_size_t(CalculateTotalWidth(), 1));

  row.AppendChar(symbols::box_drawings_heavy_vertical);

  for (const auto &column : columns) {
    auto field = column.RenderTitle();
    assert(field.GetSize() == coordinate_size_t(column.GetWidth(), 1));

    row.InsertMatrix(field);

    row.AppendChar(symbols::box_drawings_heavy_vertical);
  }

  return row;
}

game::visual::ColoredCharMatrix PlayerList::MakeDivider(bool is_first, bool is_last) {
  assert(!is_first || !is_last);

  ColoredCharMatrix row(coordinate_size_t(CalculateTotalWidth(), 1));

  if (is_first) {
    row.AppendChar(symbols::box_drawings_heavy_down_and_right);
  } else if (is_last) {
    row.AppendChar(symbols::box_drawings_heavy_up_and_right);
  } else {
    row.AppendChar(symbols::box_drawings_heavy_vertical_and_right);
  }

  for (const auto &column : columns) {
    for (int i = 0; i < column.GetWidth(); ++i) {
      row.AppendChar(symbols::box_drawings_heavy_horizontal);
    }

    if (is_first) {
      row.AppendChar(symbols::box_drawings_heavy_horizontal_and_down);
    } else if (is_last) {
      row.AppendChar(symbols::box_drawings_heavy_horizontal_and_up);
    } else {
      row.AppendChar(symbols::box_drawings_heavy_vertical_and_horizontal);
    }
  }

  Position last_position(row.GetSize().x - 1, 0);

  if (is_first) {
    row.SetChar(symbols::box_drawings_heavy_down_and_left, last_position);
  } else if (is_last) {
    row.SetChar(symbols::box_drawings_heavy_up_and_left, last_position);
  } else {
    row.SetChar(symbols::box_drawings_heavy_vertical_and_left, last_position);
  }

  return row;
}

// Subclass Column:

PlayerList::Column::Column(int width, std::string title, render_lambda_t render_lambda)
    : width(width)
    , title(std::move(title))
    , render_lambda(std::move(render_lambda)) {
}

int PlayerList::Column::GetWidth() const {
  return width;
}

game::visual::ColoredCharMatrix PlayerList::Column::RenderCell(const Row &row) const {
  game::visual::ColoredCharMatrix rendered_field(common::coordinate_size_t(width, 1));

  render_lambda(row, rendered_field);

  return rendered_field;
}

game::visual::ColoredCharMatrix PlayerList::Column::RenderTitle() const {
  game::visual::ColoredCharMatrix rendered_title(common::coordinate_size_t(width, 1));

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

  rendered_title.SetString(converter.from_bytes(title), Position(0, 0));

  return rendered_title;
}

// Subclass Row:

PlayerList::Row::Row(std::string name, int player_health, double packet_loss_percentage,
                     std::optional<std::chrono::microseconds> ping)
    : player_name(std::move(name))
    , player_health(player_health)
    , packet_loss_percentage(packet_loss_percentage)
    , ping(std::move(ping)) {
}

game::visual::ColoredCharMatrix PlayerList::Row::RenderRow(const PlayerList &p) const {
  ColoredCharMatrix row(coordinate_size_t(p.CalculateTotalWidth(), 1));

  row.AppendChar(symbols::box_drawings_heavy_vertical);

  for (const auto &column : columns) {
    auto field = column.RenderCell(*this);
    assert(field.GetSize() == coordinate_size_t(column.GetWidth(), 1));

    row.InsertMatrix(field);

    row.AppendChar(symbols::box_drawings_heavy_vertical);
  }

  return row;
}
