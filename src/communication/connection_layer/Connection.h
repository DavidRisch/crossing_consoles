#ifndef CROSSING_CONSOLES_CONNECTION_H
#define CROSSING_CONSOLES_CONNECTION_H

#include <memory>

#include "../message_layer/MessageInputStream.h"
#include "../message_layer/MessageOutputStream.h"
#include "../messages/Message.h"

/**
 * \brief Layer which is responsible for most of the TCP features.
 */
class Connection {
 public:
  /**
   * \brief Perform 3-way handshake as the client.
   */
  static std::shared_ptr<Connection> CreateClientSide(std::shared_ptr<MessageInputStream> message_input_stream,
                                                      std::shared_ptr<MessageOutputStream> message_output_stream);

  /**
   * \brief Perform 3-way handshake as the server.
   */
  static std::shared_ptr<Connection> CreateServerSide(std::shared_ptr<MessageInputStream> message_input_stream,
                                                      std::shared_ptr<MessageOutputStream> message_output_stream);

  void SendMessage(Message* message);
  std::shared_ptr<Message> ReceiveMessage();

  // TODO: send connection reset

  // TODO: send acknowledges

  // TODO: keep track of and handle timeouts

  class ConnectionCreationFailed : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "Connection could not be created.";
    }
  };

 private:
  Connection(std::shared_ptr<MessageInputStream> message_input_stream,
             std::shared_ptr<MessageOutputStream> message_output_stream);

  std::shared_ptr<MessageInputStream> message_input_stream;
  std::shared_ptr<MessageOutputStream> message_output_stream;
};

#endif  // CROSSING_CONSOLES_CONNECTION_H
