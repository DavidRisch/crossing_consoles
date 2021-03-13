#include "Connection.h"

#include <cassert>
#include <memory>
#include <thread>
#include <utility>

#include "../../message_layer/message/AcknowledgeMessage.h"
#include "../../message_layer/message/ConnectionRequestMessage.h"
#include "../../message_layer/message/ConnectionResponseMessage.h"
#include "ConnectionManager.h"

using namespace communication;
using namespace communication::connection_layer;

std::shared_ptr<message_layer::Message> Connection::ReceiveWithTimeout(
    const std::shared_ptr<message_layer::MessageInputStream> &message_input_stream,
    ProtocolDefinition::timeout_t timeout) {
  const auto start_time = std::chrono::steady_clock::now();

  while (std::chrono::steady_clock::now() - start_time <= timeout) {
    auto message = message_input_stream->ReceiveMessage(false);

    if (message != nullptr) {
      message->SetTimestampReceived(std::chrono::steady_clock::now());
      return message;
    }
  }
  throw ConnectionManager::TimeoutException();
}

std::shared_ptr<Connection> Connection::CreateClientSide(
    std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
    std::shared_ptr<message_layer::MessageOutputStream> message_output_stream, ProtocolDefinition::timeout_t timeout) {
  ProtocolDefinition::sequence_t client_sequence = 7;  // start with an arbitrary chosen sequence
  auto step_1 = message_layer::ConnectionRequestMessage();
  step_1.SetMessageSequence(client_sequence);
  client_sequence++;
  message_output_stream->SendMessage(&step_1);

  auto step_2 = ReceiveWithTimeout(message_input_stream, timeout);
  assert(step_2 != nullptr);
  if (step_2->GetMessageType() != message_layer::MessageType::CONNECTION_RESPONSE) {
    throw ConnectionCreationFailed();
  }

  message_layer::AcknowledgeMessage step_3(step_2->GetMessageSequence());
  step_3.SetMessageSequence(client_sequence);
  client_sequence++;
  message_output_stream->SendMessage(&step_3);

  return std::shared_ptr<Connection>(
      new Connection(std::move(message_input_stream), std::move(message_output_stream), client_sequence));
}

std::shared_ptr<Connection> Connection::CreateServerSide(
    std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
    std::shared_ptr<message_layer::MessageOutputStream> message_output_stream, ProtocolDefinition::timeout_t timeout) {
  ProtocolDefinition::sequence_t server_sequence = 146;  // start with an arbitrary chosen sequence
  auto step_1 = ReceiveWithTimeout(message_input_stream, timeout);
  if (step_1->GetMessageType() != message_layer::MessageType::CONNECTION_REQUEST) {
    throw ConnectionCreationFailed();
  }

  auto step_2 = message_layer::ConnectionResponseMessage();
  step_2.SetMessageSequence(server_sequence);
  server_sequence++;
  message_output_stream->SendMessage(&step_2);

  auto step_3 = ReceiveWithTimeout(message_input_stream, timeout);
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
  message->SetMessageSequence(GenerateSequence());
  message->SetTimestampSent(std::chrono::steady_clock::now());
  message_output_stream->SendMessage(message);
}

std::shared_ptr<message_layer::Message> Connection::ReceiveMessage() {
  auto received_message = message_input_stream->ReceiveMessage(false);

  // Send acknowledge for every received message except for other acknowledges
  if (received_message) {
    received_message->SetTimestampReceived(std::chrono::steady_clock::now());
    if (received_message->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
      SendAcknowledge(received_message->GetMessageSequence());
    }
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
void Connection::SendAcknowledge(ProtocolDefinition::sequence_t acknowledged_msg_sequence) {
  auto ack_msg = message_layer::AcknowledgeMessage(acknowledged_msg_sequence);
  // TODO: CloseConnection should wait for acknowledge before closing any sockets
  SendMessage(&ack_msg);
}
