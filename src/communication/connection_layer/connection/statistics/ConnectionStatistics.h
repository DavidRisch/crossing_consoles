#ifndef CROSSING_CONSOLES_CONNECTIONSTATISTICS_H
#define CROSSING_CONSOLES_CONNECTIONSTATISTICS_H

#include <list>
#include <memory>
#include <unordered_map>

#include "../../../message_layer/message/Message.h"

namespace communication::connection_layer {

/**
 * \brief Collect message statistics of a `Connection`.
 * \details Messages of type ConnectionReset, ConnectionResponse and ConnectionRequest are not captured.
 */
class ConnectionStatistics {
 public:
  ConnectionStatistics();

  // Variables used for the MessageType statistics and the calculation of the package loss
  typedef uint16_t message_count_t;
  typedef std::unordered_map<message_layer::MessageType, message_count_t> message_count_map_t;

  /**
   * \brief Holds the Statistics about the messages by MessageType and overall
   */
  struct MessageStatisticData {
    message_count_map_t count_by_type = {};
    message_count_t total_count = 0;
  };

  /**
   * \brief Holds the number of lost packages and their percentage of the overall packages
   */
  struct PackageLossData {
    message_count_t package_loss = 0;
    double package_loss_percentage = 0;
  };

  /**
   * \brief Add received message to statistics.
   */
  void AddReceivedMessage(const std::shared_ptr<message_layer::Message>& message);

  /**
   * \brief Add sent message to statistics.
   */
  void AddSentMessage(const std::shared_ptr<message_layer::Message>& message);

  /**
   * \brief Add sent and acknowledged message to statistics.
   */
  void AddSentAndAckMessage(const std::shared_ptr<message_layer::Message>& message);

  /**
   * \brief Calculate overall package loss.
   */
  PackageLossData CalculatePackageLoss() const;

  /**
   * \brief Calculate the average response time for all messages in microseconds.
   * \details Only available if at least one message has been sent.
   */
  std::optional<std::chrono::microseconds> GetAverageResponseTime() const;

  /**
   * \brief Calculate the uptime of the Connection in milliseconds.
   */
  std::chrono::microseconds CalculateUptime() const;

  MessageStatisticData GetReceivedMessageStatistics() const;

  MessageStatisticData GetSentMessageStatistics() const;

  MessageStatisticData GetSentAndAcknowledgedMessageStatistics() const;

 private:
  /**
   * \brief Holds the sum of all response intervals between time point of sent message and received acknowledge, used to
   *  calculate response time
   */
  std::chrono::microseconds response_time_sum = std::chrono::microseconds(0);

  /// Holds the total of all received messages and for each message type
  MessageStatisticData received_message_statistics;

  /// Holds the total of all sent messages and for each message type
  MessageStatisticData sent_message_statistics;

  /// Holds the total of all sent messages which have been acknowledged for each message type
  MessageStatisticData sent_and_ack_message_statistics;

  /**
   * \brief Increment overall message counter and the counter of the specific message type in message_statistics.
   */
  static void UpdateStatisticData(MessageStatisticData& message_statistics,
                                  const std::shared_ptr<message_layer::Message>& message);

  /**
   * Updates the summed response time of all sent messages in milliseconds
   * \details Adds the response interval of `message` to  `response_time_sum`.
   */
  void UpdateAverageResponseTimeSum(const std::shared_ptr<message_layer::Message>& message);

  /// Time when the Connection and the Connection statistic class were created, used for the calculation of the uptime
  std::chrono::steady_clock::time_point connection_start_time;
};
}  // namespace communication::connection_layer
#endif  // CROSSING_CONSOLES_CONNECTIONSTATISTICS_H
