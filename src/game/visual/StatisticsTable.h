#ifndef CROSSING_CONSOLES_STATISTICS_TABLE_H
#define CROSSING_CONSOLES_STATISTICS_TABLE_H

#include <functional>

#include "../../communication/connection_layer/connection/statistics/ConnectionStatistics.h"
#include "../common/Position.h"
#include "../world/World.h"
#include "ColoredCharMatrix.h"
#include "Table.h"

namespace game::visual {

/**
 * \brief A `TableRow` of `StatisticsTable`.
 */
class StatisticsTableRow : public TableRow {
 public:
  StatisticsTableRow(std::string message_type_name, int sent_count, int receive_count, int sent_and_ack_count);

  const std::string message_type_name;
  const int receive_count;
  const int sent_count;
  const int sent_and_ack_count;
};

/**
 * \brief Draws a table with `ConnectionStatistics`.
 */
class StatisticsTable : public Table<StatisticsTableRow> {
 public:
  explicit StatisticsTable(const communication::connection_layer::ConnectionStatistics &connection_statistics);

  [[nodiscard]] std::list<StatisticsTableRow> MakeRows() const override;

 private:
  const communication::connection_layer::ConnectionStatistics &connection_statistics;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_STATISTICS_TABLE_H
