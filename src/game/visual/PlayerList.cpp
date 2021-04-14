#include "PlayerList.h"

#include <codecvt>
#include <iomanip>
#include <locale>
#include <sstream>
#include <utility>

#include "HealthDisplay.h"
#include "symbols.h"

using namespace game;
using namespace game::common;
using namespace game::visual;

PlayerList::PlayerList(world::player_ptr_list_t &players)
    : players(players) {
  columns = std::list<TableColumn>{
      TableColumn(2 + GameDefinition::name_length_max, "name",
                  [](const TableRow *table_row, game::visual::ColoredCharMatrix &field) {
                    auto row = dynamic_cast<const PlayerListRow *>(table_row);
                    field.AppendChar(symbols::black_vertical_ellipse, row->player_color);
                    field.AppendChar(L' ');

                    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                    field.AppendString(converter.from_bytes(row->player_name));
                  }),
      TableColumn(HealthDisplay::width, "health",
                  [](const TableRow *table_row, game::visual::ColoredCharMatrix &field) {
                    auto row = dynamic_cast<const PlayerListRow *>(table_row);
                    field = HealthDisplay::Render(row->player_health);
                  }),
      TableColumn(5, "score",
                  [](const TableRow *table_row, game::visual::ColoredCharMatrix &field) {
                    auto row = dynamic_cast<const PlayerListRow *>(table_row);
                    field.SetString(std::to_wstring(row->player_score), Position(0, 0));
                  }),
      TableColumn(11, "packet loss",
                  [](const TableRow *table_row, game::visual::ColoredCharMatrix &field) {
                    auto row = dynamic_cast<const PlayerListRow *>(table_row);
                    std::wstringstream stream;
                    stream << std::fixed << std::setprecision(1) << row->packet_loss_percentage << L"%";

                    field.SetString(stream.str(), Position(0, 0));
                  }),
      TableColumn(10, "ping",
                  [](const TableRow *table_row, game::visual::ColoredCharMatrix &field) {
                    auto row = dynamic_cast<const PlayerListRow *>(table_row);
                    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

                    if (row->ping.has_value()) {
                      auto ping_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(*row->ping).count();
                      field.AppendString(converter.from_bytes(std::to_string(ping_in_ms) + " ms"));
                    }
                  })

  };
}

std::list<PlayerListRow> PlayerList::MakeRows() const {
  std::list<PlayerListRow> rows;
  for (const auto &player : players) {
    PlayerListRow row(player->name, player->color, player->health, player->GetScore(), player->packet_loss_percentage,
                      player->ping);
    rows.emplace_back(player->name, player->color, player->health, player->GetScore(), player->packet_loss_percentage,
                      player->ping);
  }
  return rows;
}

PlayerListRow::PlayerListRow(std::string name, common::Color player_color, int player_health, uint16_t player_score,
                             double packet_loss_percentage, std::optional<std::chrono::microseconds> ping)
    : player_name(std::move(name))
    , player_color(std::move(player_color))
    , player_health(player_health)
    , player_score(player_score)
    , packet_loss_percentage(packet_loss_percentage)
    , ping(ping) {
}
