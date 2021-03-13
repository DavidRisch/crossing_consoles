#ifndef CROSSING_CONSOLES_CONNECTION_H
#define CROSSING_CONSOLES_CONNECTION_H

#include <memory>

#include "../../message_layer/message/Message.h"
#include "../../message_layer/message_stream/MessageInputStream.h"
#include "../../message_layer/message_stream/MessageOutputStream.h"

namespace communication {
namespace connection_layer {

/**
 * \brief Layer which is responsible for most of the TCP features.
 */
class Connection {
 public:
  /**
   * \brief Perform 3-way handshake as the client.
   */
  static std::shared_ptr<Connection> CreateClientSide(
      std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
      std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
      ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout);

  /**
   * \brief Perform 3-way handshake as the server.
   */
  static std::shared_ptr<Connection> CreateServerSide(
      std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
      std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
      ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout);

  /**
   * \brief Send message
   */
  void SendMessage(message_layer::Message* message);
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

 private:
  Connection(std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
             std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
             ProtocolDefinition::sequence_t sequence_counter);

  /**
   *
   * \brief Check for timeout while receiving messages
   */
  static std::shared_ptr<message_layer::Message> ReceiveWithTimeout(
      const std::shared_ptr<message_layer::MessageInputStream>& message_input_stream,
      ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout);

  std::shared_ptr<message_layer::MessageInputStream> message_input_stream;
  std::shared_ptr<message_layer::MessageOutputStream> message_output_stream;

  /**
   * \brief Send acknowledge message for a received message identified by its sequence to the specified address.
   */
  void SendAcknowledge(ProtocolDefinition::sequence_t sequence);

  /**
   * \brief Return current sequence count and increment sequence counter.
   */
  ProtocolDefinition::sequence_t GenerateSequence();
  ProtocolDefinition::sequence_t sequence_counter;
};

}  // namespace connection_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTION_H
