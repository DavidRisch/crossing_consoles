#include "Connection.h"

#include <memory>
#include <thread>
#include <utility>

#include "../message_layer/message/AcknowledgeMessage.h"
#include "../message_layer/message/ConnectionRequestMessage.h"
#include "../message_layer/message/ConnectionResponseMessage.h"

using namespace communication;
using namespace communication::connection_layer;

std::shared_ptr<Connection> Connection::CreateClientSide(
    std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
    std::shared_ptr<message_layer::MessageOutputStream> message_output_stream) {
  ProtocolDefinition::sequence_t client_sequence = 0;
  message_layer::ConnectionRequestMessage step_1(1234, client_sequence);  // TODO: address
  message_output_stream->SendMessage(&step_1);

  // TODO: some kind of timeout
  auto step_2 = message_input_stream->ReceiveMessage();
  if (step_2->GetMessageType() != message_layer::MessageType::CONNECTION_RESPONSE) {
    throw ConnectionCreationFailed();
  }

  message_layer::AcknowledgeMessage step_3(1234, step_2->GetMessageSequence());  // TODO: address
  message_output_stream->SendMessage(&step_3);
  return std::shared_ptr<Connection>(
      new Connection(std::move(message_input_stream), std::move(message_output_stream), ++client_sequence));
}

std::shared_ptr<Connection> Connection::CreateServerSide(
    std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
    std::shared_ptr<message_layer::MessageOutputStream> message_output_stream) {
  // TODO: some kind of timeout
  ProtocolDefinition::sequence_t server_sequence = 0;
  auto step_1 = message_input_stream->ReceiveMessage();

  if (step_1->GetMessageType() != message_layer::MessageType::CONNECTION_REQUEST) {
    throw ConnectionCreationFailed();
  }

  message_layer::ConnectionResponseMessage step_2(1234, server_sequence + 2);  // TODO: address
  message_output_stream->SendMessage(&step_2);

  // TODO: some kind of timeout
  auto step_3 = message_input_stream->ReceiveMessage();
  if (step_3->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
    throw ConnectionCreationFailed();
  } else {
    auto *step_3_ack = dynamic_cast<message_layer::AcknowledgeMessage *>(step_3.get());
    if (step_3_ack->GetAcknowledgedMessageSequence() != step_2.GetMessageSequence()) {
      throw ConnectionCreationFailed();
    }
  }

  return std::shared_ptr<Connection>(
      new Connection(std::move(message_input_stream), std::move(message_output_stream), server_sequence));
}

void Connection::SendMessage(message_layer::Message *message) {
  message_output_stream->SendMessage(message);
}

std::shared_ptr<message_layer::Message> Connection::ReceiveMessage() {
  // TODO: set address
  auto received_message = message_input_stream->ReceiveMessage();

  // TODO: Handle connection reset by other side
  // TODO: Set MetaData

  // Send acknowledge for every received message except for other acknowledges
  if (received_message->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
    SendAcknowledge(received_message->GetAddress(), received_message->GetMessageSequence());
  }

  return received_message;
}

Connection::Connection(std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
                       std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
                       ProtocolDefinition::sequence_t sequence_counter)
    : message_input_stream(std::move(message_input_stream))
    , message_output_stream(std::move(message_output_stream))
    , sequence_counter(sequence_counter) {
}

ProtocolDefinition::sequence_t Connection::GenerateSequence() {
  ProtocolDefinition::sequence_t current_counter = sequence_counter;
  sequence_counter++;
  return current_counter;
}
void Connection::SendAcknowledge(message_layer::address_t address, ProtocolDefinition::sequence_t sequence) {
  auto ack_msg = message_layer::AcknowledgeMessage(address, sequence, GenerateSequence());
  SendMessage(&ack_msg);
}
