#include "Connection.h"

#include <thread>
#include <utility>

#include "../messages/ConnectionRequestMessage.h"
#include "../messages/ConnectionResponseMessage.h"

std::shared_ptr<Connection> Connection::CreateClientSide(std::shared_ptr<MessageInputStream> message_input_stream,
                                                         std::shared_ptr<MessageOutputStream> message_output_stream) {
  ConnectionRequestMessage step_1(1234);  // TODO: address
  message_output_stream->SendMessage(&step_1);

  // TODO: some kind of timeout
  auto step_2 = message_input_stream->ReceiveMessage();
  if (step_2->GetMessageType() != MessageType::CONNECTION_RESPONSE) {
    throw ConnectionCreationFailed();
  }

  ConnectionResponseMessage step_3(1234);  // TODO: address
  message_output_stream->SendMessage(&step_3);

  return std::shared_ptr<Connection>(new Connection(std::move(message_input_stream), std::move(message_output_stream)));
}

std::shared_ptr<Connection> Connection::CreateServerSide(std::shared_ptr<MessageInputStream> message_input_stream,
                                                         std::shared_ptr<MessageOutputStream> message_output_stream) {
  // TODO: some kind of timeout
  auto step_1 = message_input_stream->ReceiveMessage();
  if (step_1->GetMessageType() != MessageType::CONNECTION_REQUEST) {
    throw ConnectionCreationFailed();
  }

  ConnectionResponseMessage step_2(1234);  // TODO: address
  message_output_stream->SendMessage(&step_2);

  // TODO: some kind of timeout
  auto step_3 = message_input_stream->ReceiveMessage();
  if (step_3->GetMessageType() != MessageType::CONNECTION_RESPONSE) {
    throw ConnectionCreationFailed();
  }

  return std::shared_ptr<Connection>(new Connection(std::move(message_input_stream), std::move(message_output_stream)));
}

void Connection::SendMessage(Message *message) {
  message_output_stream->SendMessage(message);
}

std::shared_ptr<Message> Connection::ReceiveMessage() {
  // TODO: set address
  auto received_message = message_input_stream->ReceiveMessage();

  // TODO: Handle connection reset by other side

  return received_message;
}

Connection::Connection(std::shared_ptr<MessageInputStream> message_input_stream,
                       std::shared_ptr<MessageOutputStream> message_output_stream)
    : message_input_stream(std::move(message_input_stream))
    , message_output_stream(std::move(message_output_stream)) {
}
