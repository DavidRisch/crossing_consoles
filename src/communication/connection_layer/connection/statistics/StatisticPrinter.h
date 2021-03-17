#ifndef CROSSING_CONSOLES_STATISTICPRINTER_H
#define CROSSING_CONSOLES_STATISTICPRINTER_H

#include "ConnectionStatistics.h"

namespace communication::connection_layer {
class StatisticPrinter {
  /**
   * \brief Print message statistics of a `Connection` collected by `ConnectionStatistic` to console.
   */

 public:
  StatisticPrinter();

  /**
   * \brief Print statistics to console.
   * \details Prints message count (sent, received) for each message type plus package loss, average response time and
   * uptime of the Connection.
   */
  static void PrintStatistics(ConnectionStatistics statistics);

 private:
  /**
   * \brief Print message count for each Message Type to console.
   */
  static void PrintMapStatistics(const ConnectionStatistics::MessageStatisticData& sent_message_statistics,
                                 const ConnectionStatistics::MessageStatisticData& received_message_statistics);

  /**
   * \brief Print overall package loss to console.
   */
  static void PrintPackageLoss(ConnectionStatistics::PackageLossData data);

  /**
   * \brief Print the average response time in milliseconds
   */
  static void PrintAverageResponseTime(double response_time);

  /**
   * \brief Print the uptime of the Connection in milliseconds
   */
  static void PrintUptime(double uptime);

  /// Mapping from MessageTypes to Strings, used for printing of the statistics
  static const std::unordered_map<message_layer::MessageType, const std::string> map_message_type_to_string;
};
}  // namespace communication::connection_layer

#endif  // CROSSING_CONSOLES_STATISTICPRINTER_H
