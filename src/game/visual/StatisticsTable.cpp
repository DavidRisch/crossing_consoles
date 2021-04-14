#include "StatisticsTable.h"

#include <codecvt>
#include <locale>
#include <utility>

#include "../../communication/connection_layer/connection/statistics/StatisticPrinter.h"

using namespace game;
using namespace game::common;
using namespace game::visual;

StatisticsTable::StatisticsTable(const communication::connection_layer::ConnectionStatistics &connection_statistics)
    : connection_statistics(connection_statistics) {
  int column_width = 21;

  columns = std::list<TableColumn>{
      TableColumn(column_width, "message type",
                  [](const TableRow *table_row, game::visual::ColoredCharMatrix &field) {
                    auto row = dynamic_cast<const StatisticsTableRow *>(table_row);
                    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                    field.AppendString(converter.from_bytes(row->message_type_name));
                  }),
      TableColumn(column_width, "received",
                  [](const TableRow *table_row, game::visual::ColoredCharMatrix &field) {
                    auto row = dynamic_cast<const StatisticsTableRow *>(table_row);
                    field.SetString(std::to_wstring(row->receive_count), Position(0, 0));
                  }),
      TableColumn(column_width, "sent",
                  [](const TableRow *table_row, game::visual::ColoredCharMatrix &field) {
                    auto row = dynamic_cast<const StatisticsTableRow *>(table_row);
                    field.SetString(std::to_wstring(row->sent_count), Position(0, 0));
                  }),
      TableColumn(column_width, "sent and acknowledged",
                  [](const TableRow *table_row, game::visual::ColoredCharMatrix &field) {
                    auto row = dynamic_cast<const StatisticsTableRow *>(table_row);
                    field.SetString(std::to_wstring(row->sent_and_ack_count), Position(0, 0));
                  }),

  };
}

std::list<StatisticsTableRow> StatisticsTable::MakeRows() const {
  std::list<StatisticsTableRow> rows;

  const auto &sent_message_statistics = connection_statistics.GetSentMessageStatistics();
  const auto &received_message_statistics = connection_statistics.GetReceivedMessageStatistics();
  const auto &sent_and_ack_message_statistics = connection_statistics.GetSentAndAcknowledgedMessageStatistics();

  for (auto &entry : communication::connection_layer::StatisticPrinter::map_message_type_to_string) {
    int sent_entry = 0;
    int received_entry = 0;
    int sent_and_ack_entry = 0;

    if (sent_message_statistics.count_by_type.find(entry.first) != sent_message_statistics.count_by_type.end()) {
      sent_entry = (sent_message_statistics.count_by_type.find(entry.first))->second;
    }

    if (sent_and_ack_message_statistics.count_by_type.find(entry.first) !=
        sent_and_ack_message_statistics.count_by_type.end()) {
      sent_and_ack_entry = (sent_and_ack_message_statistics.count_by_type.find(entry.first))->second;
    }

    if (received_message_statistics.count_by_type.find(entry.first) !=
        received_message_statistics.count_by_type.end()) {
      received_entry = (received_message_statistics.count_by_type.find(entry.first))->second;
    }

    rows.emplace_back(entry.second, received_entry, sent_entry, sent_and_ack_entry);
  }

  rows.emplace_back("SUM", received_message_statistics.total_count, sent_message_statistics.total_count,
                    sent_and_ack_message_statistics.total_count);

  return rows;
}

StatisticsTableRow::StatisticsTableRow(std::string message_type_name, int receive_count, int sent_count,
                                       int sent_and_ack_count)
    : message_type_name(std::move(message_type_name))
    , receive_count(receive_count)
    , sent_count(sent_count)
    , sent_and_ack_count(sent_and_ack_count) {
}
