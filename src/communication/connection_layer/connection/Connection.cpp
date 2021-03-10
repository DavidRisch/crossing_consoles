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
    const std::shared_ptr<message_layer::MessageInputStream> &message_input_stream, timeout_t timeout) {
  const auto start_time = std::chrono::steady_clock::now();

  while (std::chrono::steady_clock::now() - start_time <= timeout) {
    auto message = message_input_stream->ReceiveMessage(false);

    if (message != nullptr) {
      return message;
    }
  }
  throw ConnectionManager::TimeoutException();
}

std::shared_ptr<Connection> Connection::CreateClientSide(
    std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
    std::shared_ptr<message_layer::MessageOutputStream> message_output_stream, timeout_t timeout) {
  sequence_t client_sequence = 1200;                     // start with an arbitrarily chosen sequence
  message_layer::ConnectionRequestMessage step_1(1234);  // TODO: address
  step_1.SetMessageSequence(client_sequence);
  client_sequence++;
  message_output_stream->SendMessage(&step_1);

  return std::shared_ptr<Connection>(new Connection(std::move(message_input_stream), std::move(message_output_stream),
                                                    ConnectionState::CLIENT_CONNECTION_REQUEST_SEND, client_sequence,
                                                    timeout));
}

std::shared_ptr<Connection> Connection::CreateServerSide(
    std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
    std::shared_ptr<message_layer::MessageOutputStream> message_output_stream, timeout_t timeout) {
  return std::shared_ptr<Connection>(new Connection(std::move(message_input_stream), std::move(message_output_stream),
                                                    ConnectionState::SERVER_WAITING_FOR_FIRST,
                                                    3400,  // start with an arbitrarily chosen sequence
                                                    timeout));
}

bool Connection::TryEstablish() {
  // TODO: implement in a none blocking way
  switch (state) {
    case (ConnectionState::CLIENT_CONNECTION_REQUEST_SEND): {
      auto step_2 = ReceiveWithTimeout(message_input_stream, timeout);
      assert(step_2 != nullptr);
      if (step_2->GetMessageType() != message_layer::MessageType::CONNECTION_RESPONSE) {
        throw ConnectionCreationFailed();
      }

      message_layer::AcknowledgeMessage step_3(1234, step_2->GetMessageSequence());
      message_output_stream->SendMessage(&step_3);

      state = ConnectionState::ESTABLISHED;
      break;
    }
    case (ConnectionState::SERVER_WAITING_FOR_FIRST): {
      auto step_1 = ReceiveWithTimeout(message_input_stream, timeout);
      if (step_1->GetMessageType() != message_layer::MessageType::CONNECTION_REQUEST) {
        throw ConnectionCreationFailed();
      }

      message_layer::ConnectionResponseMessage step_2(1234);
      last_send_sequence = GenerateSequence();
      step_2.SetMessageSequence(last_send_sequence);
      message_output_stream->SendMessage(&step_2);

      state = ConnectionState::SERVER_CONNECTION_REQUEST_SEND;
      break;
    }
    case (ConnectionState::SERVER_CONNECTION_REQUEST_SEND): {
      auto step_3 = ReceiveWithTimeout(message_input_stream, timeout);
      if (step_3->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
        throw ConnectionCreationFailed();
      } else {
        auto *step_3_ack = dynamic_cast<message_layer::AcknowledgeMessage *>(step_3.get());
        if (step_3_ack->GetAcknowledgedMessageSequence() != last_send_sequence) {
          throw ConnectionCreationFailed();
        }
      }
      state = ConnectionState::ESTABLISHED;
      break;
    }
    default:
      throw std::runtime_error("Invalid ConnectionState");
  }

  return state == ConnectionState::ESTABLISHED;
}

void Connection::BlockingEstablish() {
  for (int i = 0; i < 100; ++i) {
    if (TryEstablish()) {
      assert(state == ConnectionState::ESTABLISHED);
      return;
    }
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
  throw std::runtime_error("BlockingEstablish() took to long");
}

void Connection::SendMessage(message_layer::Message *message) {
  assert(state == ConnectionState::ESTABLISHED);

  last_send_sequence = GenerateSequence();
  message->SetMessageSequence(last_send_sequence);
  message_output_stream->SendMessage(message);
}

std::shared_ptr<message_layer::Message> Connection::ReceiveMessage() {
  assert(state == ConnectionState::ESTABLISHED);

  auto received_message = message_input_stream->ReceiveMessage(false);

  // TODO: Set MetaData

  // Send acknowledge for every received message except for other acknowledges
  if (received_message != nullptr) {
    if (received_message->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
      SendAcknowledge(received_message->GetAddress(), received_message->GetMessageSequence());
    }
  }

  return received_message;
}

Connection::Connection(std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
                       std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
                       ConnectionState connection_state, sequence_t sequence_counter, timeout_t timeout)
    : sequence_counter(sequence_counter)
    , state(connection_state)
    , timeout(timeout)
    , message_input_stream(std::move(message_input_stream))
    , message_output_stream(std::move(message_output_stream)) {
}

sequence_t Connection::GenerateSequence() {
  sequence_t current_counter = sequence_counter;
  sequence_counter++;
  return current_counter;
}

void Connection::SendAcknowledge(message_layer::address_t address, sequence_t acknowledged_msg_sequence) {
  auto ack_msg = message_layer::AcknowledgeMessage(address, acknowledged_msg_sequence);
  SendMessage(&ack_msg);
}
