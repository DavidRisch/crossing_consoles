#ifndef CROSSING_CONSOLES_CONNECTION_H
#define CROSSING_CONSOLES_CONNECTION_H

#include <memory>

#include "../message_layer/message/Message.h"
#include "../message_layer/message_stream/MessageInputStream.h"
#include "../message_layer/message_stream/MessageOutputStream.h"

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
      std::shared_ptr<message_layer::MessageOutputStream> message_output_stream);

  /**
   * \brief Perform 3-way handshake as the server.
   */
  static std::shared_ptr<Connection> CreateServerSide(
      std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
      std::shared_ptr<message_layer::MessageOutputStream> message_output_stream);

  void SendMessage(message_layer::Message* message);
  std::shared_ptr<message_layer::Message> ReceiveMessage();

  // TODO: send connection reset

  // TODO: send acknowledges

  // TODO: keep track of and handle timeouts

  class ConnectionCreationFailed : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "Connection could not be created.";
    }
  };

 private:
  Connection(std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
             std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
             ProtocolDefinition::sequence_t sequence_counter);

  void SendAcknowledge(message_layer::address_t address, ProtocolDefinition::sequence_t sequence);

  ProtocolDefinition::sequence_t GenerateSequence();

  ProtocolDefinition::sequence_t sequence_counter;

  std::shared_ptr<message_layer::MessageInputStream> message_input_stream;
  std::shared_ptr<message_layer::MessageOutputStream> message_output_stream;
};

}  // namespace connection_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTION_H
