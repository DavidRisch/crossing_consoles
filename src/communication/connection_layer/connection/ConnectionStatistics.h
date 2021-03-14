#ifndef CROSSING_CONSOLES_CONNECTIONSTATISTICS_H
#define CROSSING_CONSOLES_CONNECTIONSTATISTICS_H

#include <unordered_map>

#include "../../message_layer/message/Message.h"

namespace communication::connection_layer {

class ConnectionStatistics {
  /**
   * \brief Collect and display message statistics of a `Connection`.
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
   * \brief Print statistics to console.
   * \details Prints message count (sent, received) for each message type and package loss.
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

  std::unordered_map<message_layer::MessageType, uint16_t> received_message_map = {};
  uint16_t received_message_count = 0;
  uint16_t sent_acknowledge_count = 0;
  uint16_t received_acknowledge_count = 0;
  std::unordered_map<message_layer::MessageType, uint16_t> sent_message_map = {};
  uint16_t sent_message_count = 0;
  int package_loss = 0;
  double package_loss_percentage = 0;

  static const std::unordered_map<message_layer::MessageType, const std::string> map_message_type_to_string;
};

}  // namespace communication::connection_layer
#endif  // CROSSING_CONSOLES_CONNECTIONSTATISTICS_H
