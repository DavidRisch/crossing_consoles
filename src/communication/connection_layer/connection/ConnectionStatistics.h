#ifndef CROSSING_CONSOLES_CONNECTIONSTATISTICS_H
#define CROSSING_CONSOLES_CONNECTIONSTATISTICS_H

#include <list>
#include <unordered_map>

#include "../../message_layer/message/Message.h"

namespace communication::connection_layer {

class ConnectionStatistics {
  /**
   * \brief Collect and display message statistics of a `Connection`.
   * \details Messages of type ConnectionReset, ConnectionResponse and ConnectionRequest are not captured.
   */

 public:
  ConnectionStatistics();

  /**
   * \brief Add received message to statistics.
   */
  void AddReceivedMessage(const message_layer::Message& message);

  /**
   * \brief Add sent message to statistics.
   */
  void AddSentMessage(const message_layer::Message& message);

  /**
   * \brief Set the timestamp at which the sent message was received after it has been acknowledged
   */
  void SetReceivedTimestampForSentMessage(ProtocolDefinition::sequence_t sequence);

  /**
   * \brief Print statistics to console.
   * \details Prints message count (sent, received) for each message type plus package loss, average response time and
   * uptime of the Connection.
   */
  void PrintStatistics();

 private:
  /**
   * \brief Print message count for each Message Type to console.
   */
  void PrintMapStatistics();

  /**
   * \brief Calculate overall package loss.
   */
  void CalculatePackageLoss();

  /**
   * \brief Print overall package loss to console.
   */
  void PrintPackageLoss() const;

  /**
   * \brief Calculate the average response time for all Messages in milliseconds
   */
  double CalculateAverageResponseTime();

  /**
   * \brief Print the average response time in milliseconds
   */
  void PrintAverageResponseTime();

  /**
   * \brief Calculate the uptime of the Connection in milliseconds
   */
  double CalculateUptime();

  /**
   * \brief Print the uptime of the Connection in milliseconds
   */
  void PrintUptime();

  // Variables used for the MessageType statistics and the calculation of the package loss
  typedef uint16_t message_count_t;
  typedef std::unordered_map<message_layer::MessageType, message_count_t> message_count_map_t;

  /// Struct to hold the total of all messages and for each message type
  struct MessageStatisticData {
    message_count_map_t map = {};
    message_count_t count = 0;
  };

  /// Hold the total of all received messages and for each message type
  MessageStatisticData received_message_statistics;

  /// Hold the total of all sent messages and for each message type
  MessageStatisticData sent_message_statistics;

  /**
   * \brief Increment overall message counter and of specific message type in message_statistic
   */
  static void UpdateStatisticData(MessageStatisticData& message_statistics, const message_layer::Message& message);

  message_count_t package_loss = 0;
  double package_loss_percentage = 0;

  /// List of all sent messages which is used to calculate the average response time
  std::list<message_layer::Message> sent_message_list;

  /// Time when the Connection and the Connection statistic class were created, used for the calculation of the uptime
  std::chrono::steady_clock::time_point connection_start_time;

  /// Mapping from MessageTypes to Strings, used for printing of the statistics
  static const std::unordered_map<message_layer::MessageType, const std::string> map_message_type_to_string;
};

}  // namespace communication::connection_layer
#endif  // CROSSING_CONSOLES_CONNECTIONSTATISTICS_H
