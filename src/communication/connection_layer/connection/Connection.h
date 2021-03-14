#ifndef CROSSING_CONSOLES_CONNECTION_H
#define CROSSING_CONSOLES_CONNECTION_H

#include <memory>
#include <queue>

#include "../../message_layer/message/Message.h"
#include "../../message_layer/message_stream/MessageInputStream.h"
#include "../../message_layer/message_stream/MessageOutputStream.h"
#include "ConnectionStatistics.h"

namespace communication {
namespace connection_layer {

enum class ConnectionState {
  CLIENT_CONNECTION_REQUEST_SENT,
  SERVER_WAITING_FOR_FIRST,
  SERVER_CONNECTION_RESPONSE_SENT,
  READY,
  WAITING_FOR_ACKNOWLEDGE,
};

using sequence_t = ProtocolDefinition::sequence_t;
using timeout_t = ProtocolDefinition::timeout_t;

/**
 * \brief Layer which is responsible for most of the TCP features.
 */
class Connection {
 public:
  /**
   * \brief Start the 3-way handshake as the client.
   */
  static std::shared_ptr<Connection> CreateClientSide(
      std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
      std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
      timeout_t timeout = ProtocolDefinition::timeout);

  /**
   * \brief Start the 3-way handshake as the server.
   */
  static std::shared_ptr<Connection> CreateServerSide(
      std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
      std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
      timeout_t timeout = ProtocolDefinition::timeout);

  /**
   * \brief Perform a step in the 3-way handshake, must be called until it returns true.
   */
  bool TryEstablish();

  /**
   * \brief Perform the complete 3-way handshake, intended for testing.
   */
  void BlockingEstablish();

  void Handle();

  /**
   * \brief Add a message to the send queue and call Handle.
   * \details Iff the `send_message_queue` is empty, the message will be send immediately.
   */
  void SendMessage(const std::shared_ptr<message_layer::Message>& message);

  /**
   * \brief Receive and message and send acknowledge message.
   * \details Sends acknowledge message only if received message type is not acknowledge.
   */
  std::shared_ptr<message_layer::Message> ReceiveMessage();

  class ConnectionCreationFailed : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "Connection could not be created.";
    }
  };

  /**
   * \brief Thrown if an acknowledge is received at the wrong time or with the wrong content.
   */
  class BadAcknowledgeException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "Bad acknowledge received.";
    }
  };

  /**
   * Print connection statistics
   */
  void PrintStatistics();

 private:
  Connection(std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
             std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
             ConnectionState connection_state, sequence_t sequence_counter, timeout_t timeout);

  /**
   * \brief Send a message.
   */
  void SendMessageNow(message_layer::Message* message);

  /**
   *
   * \brief Check for timeout while receiving messages
   */
  static std::shared_ptr<message_layer::Message> ReceiveWithTimeout(
      const std::shared_ptr<message_layer::MessageInputStream>& message_input_stream,
      timeout_t timeout = ProtocolDefinition::timeout);

  /// The sequence number of the last send message not including acknowledge messages.
  sequence_t last_send_sequence{};

  ConnectionState state;

  timeout_t timeout;

  /// Unprocessed events ordered from oldest to newest.
  std::queue<std::shared_ptr<message_layer::Message>> send_message_queue;

  std::shared_ptr<message_layer::MessageInputStream> message_input_stream;
  std::shared_ptr<message_layer::MessageOutputStream> message_output_stream;

  /**
   * \brief Send acknowledge message for a received message identified by its sequence to the specified address.
   */
  void SendAcknowledge(sequence_t sequence);

  /**
   * \brief Return current sequence count and increment sequence counter.
   */
  sequence_t GenerateSequence();
  sequence_t sequence_counter;

  ConnectionStatistics statistics = ConnectionStatistics();
};

}  // namespace connection_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTION_H
