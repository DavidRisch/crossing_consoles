#ifndef CROSSING_CONSOLES_CONNECTION_H
#define CROSSING_CONSOLES_CONNECTION_H

#include <list>
#include <memory>
#include <queue>

#include "../../message_layer/message/Message.h"
#include "../../message_layer/message_stream/MessageInputStream.h"
#include "../../message_layer/message_stream/MessageOutputStream.h"
#include "statistics/ConnectionStatistics.h"
#include "statistics/StatisticPrinter.h"

namespace communication {
namespace connection_layer {

enum class ConnectionState {
  CLIENT_CONNECTION_START,
  CLIENT_CONNECTION_REQUEST_SENT,
  SERVER_WAITING_FOR_FIRST,
  SERVER_CONNECTION_RESPONSE_SENT,
  READY,
  WAITING_FOR_ACKNOWLEDGE,
  WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE,
  CLOSED,
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

  /**
   * \brief Thrown if something goes wrong during the handshake so the Connection can not be established.
   */
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
   * \brief Return connection statistics
   */
  const ConnectionStatistics& GetConnectionStatistics() const;

  /**
   * \brief Returns true if connection is in state `CLOSED`
   */
  bool ConnectionClosed() const;

 private:
  Connection(std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
             std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
             ConnectionState connection_state, sequence_t sequence_counter, timeout_t timeout);

  /**
   * \brief Send a message.
   */
  void SendMessageNow(const std::shared_ptr<message_layer::Message>& message, bool expect_acknowledge = true);

  /**
   * Calls `ResendLastMessages()` if the last messages probably has not been received.
   */
  void ResendIfNecessary();

  /**
   * \brief Resends all Messages which might not have been received.
   */
  void ResendLastMessages();

  /**
   *
   * \brief Check for timeout while receiving messages
   */
  std::shared_ptr<message_layer::Message> ReceiveWithTimeout();

  /**
   * \brief Receives 1 or 0 new messages and adds them to the `receive_message_queue`.
   * \brief Returns true iff another call might receive the next message.
   */
  bool TryReceive();

  /**
   * \brief Call `TryReceive()` until no new messages can be received.
   */
  void ReceiveAll();

  /// The sequence number of the last send message not including acknowledge and NACK messages.
  sequence_t last_send_sequence{};

  ConnectionState state;

  timeout_t timeout;

  /// Unsent Messages ordered from oldest to newest.
  std::queue<std::shared_ptr<message_layer::Message>> send_message_queue;
  /// Received Messages ordered from oldest to newest.
  std::queue<std::shared_ptr<message_layer::Message>> receive_message_queue;

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

  /// How frequently Messages which might not have been received by the other side should be resend.
  ProtocolDefinition::timeout_t resend_interval{};
  /// Reset every time a valid message (except NACK) is received, a message is send or a message is resend.
  std::chrono::time_point<std::chrono::steady_clock> timestamp_last_change;

  /**
   * \brief Messages which might not have been received by the other side (no acknowledge received yet)
   * ordered from oldest to newest.
   */
  std::list<std::shared_ptr<message_layer::Message>> unacknowledged_sent_message;

  /// The newest sequence of a correct Message received from the other side.
  std::optional<sequence_t> last_received_sequence_counter;

  ConnectionStatistics statistics{};
};

}  // namespace connection_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTION_H
