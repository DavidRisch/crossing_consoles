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
      message->SetTimestampReceived(std::chrono::steady_clock::now());
      return message;
    }
  }
  throw ConnectionManager::TimeoutException();
}

std::shared_ptr<Connection> Connection::CreateClientSide(
    std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
    std::shared_ptr<message_layer::MessageOutputStream> message_output_stream, timeout_t timeout) {
  sequence_t client_sequence = 1200;  // start with an arbitrarily chosen sequence
  auto step_1 = message_layer::ConnectionRequestMessage();
  step_1.SetMessageSequence(client_sequence);
  client_sequence++;
  message_output_stream->SendMessage(&step_1);

  return std::shared_ptr<Connection>(new Connection(std::move(message_input_stream), std::move(message_output_stream),
                                                    ConnectionState::CLIENT_CONNECTION_REQUEST_SENT, client_sequence,
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
    case (ConnectionState::CLIENT_CONNECTION_REQUEST_SENT): {
      auto step_2 = ReceiveWithTimeout(message_input_stream, timeout);
      assert(step_2 != nullptr);
      if (step_2->GetMessageType() != message_layer::MessageType::CONNECTION_RESPONSE) {
        throw ConnectionCreationFailed();
      }

      message_layer::AcknowledgeMessage step_3(step_2->GetMessageSequence());
      message_output_stream->SendMessage(&step_3);

      state = ConnectionState::READY;
      break;
    }
    case (ConnectionState::SERVER_WAITING_FOR_FIRST): {
      auto step_1 = ReceiveWithTimeout(message_input_stream, timeout);
      if (step_1->GetMessageType() != message_layer::MessageType::CONNECTION_REQUEST) {
        throw ConnectionCreationFailed();
      }

      message_layer::ConnectionResponseMessage step_2;
      last_send_sequence = GenerateSequence();
      step_2.SetMessageSequence(last_send_sequence);
      message_output_stream->SendMessage(&step_2);

      state = ConnectionState::SERVER_CONNECTION_RESPONSE_SENT;
      break;
    }
    case (ConnectionState::SERVER_CONNECTION_RESPONSE_SENT): {
      auto step_3 = ReceiveWithTimeout(message_input_stream, timeout);
      if (step_3->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
        throw ConnectionCreationFailed();
      } else {
        auto *step_3_ack = dynamic_cast<message_layer::AcknowledgeMessage *>(step_3.get());
        if (step_3_ack->GetAcknowledgedMessageSequence() != last_send_sequence) {
          throw ConnectionCreationFailed();
        }
      }
      state = ConnectionState::READY;
      break;
    }
    default:
      throw std::runtime_error("Invalid ConnectionState");
  }

  return state == ConnectionState::READY;
}

void Connection::BlockingEstablish() {
  for (int i = 0; i < 100; ++i) {
    if (TryEstablish()) {
      assert(state == ConnectionState::READY);
      return;
    }
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
  throw std::runtime_error("BlockingEstablish() took to long");
}

void Connection::SendMessage(const std::shared_ptr<message_layer::Message> &message) {
  if (state == ConnectionState::CLOSED) {
    // do not send any messages if connection is closed
    return;
  }
  assert(state == ConnectionState::READY || state == ConnectionState::WAITING_FOR_ACKNOWLEDGE);
  send_message_queue.push(message);

  Handle();
}

std::shared_ptr<message_layer::Message> Connection::ReceiveMessage() {
  if (state == ConnectionState::CLOSED) {
    // do not receive any messages if connection is closed
    return nullptr;
  }
  assert(state == ConnectionState::READY || state == ConnectionState::WAITING_FOR_ACKNOWLEDGE ||
         state == ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE);

  auto received_message = message_input_stream->ReceiveMessage(false);

  if (received_message != nullptr) {
    // set Metadata
    received_message->SetTimestampReceived(std::chrono::steady_clock::now());

    if (received_message->GetMessageType() == message_layer::MessageType::ACKNOWLEDGE) {
      if (!(state == ConnectionState::WAITING_FOR_ACKNOWLEDGE ||
            state == ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE)) {
        throw BadAcknowledgeException();
      }
      auto acknowledge_message = std::dynamic_pointer_cast<message_layer::AcknowledgeMessage>(received_message);
      if (acknowledge_message->GetAcknowledgedMessageSequence() != last_send_sequence) {
        throw BadAcknowledgeException();
      }

      if (state == ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE) {
        // acknowledge of ConnectionResetMessage has been received, close connection
        state = ConnectionState::CLOSED;
      } else {
        state = ConnectionState::READY;
      }

    } else {
      // Send acknowledge for every received message except for other acknowledges
      SendAcknowledge(received_message->GetMessageSequence());
    }
  }

  return received_message;
}

Connection::Connection(std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
                       std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
                       ConnectionState connection_state, sequence_t sequence_counter, timeout_t timeout)
    : state(connection_state)
    , sequence_counter(sequence_counter)
    , timeout(timeout)
    , message_input_stream(std::move(message_input_stream))
    , message_output_stream(std::move(message_output_stream)) {
}

sequence_t Connection::GenerateSequence() {
  sequence_t current_counter = sequence_counter;
  sequence_counter++;
  return current_counter;
}

void Connection::SendMessageNow(message_layer::Message *message) {
  assert(state == ConnectionState::READY || (message->GetMessageType() == message_layer::MessageType::ACKNOWLEDGE &&
                                             state == ConnectionState::WAITING_FOR_ACKNOWLEDGE));

  auto send_sequence = GenerateSequence();
  if (message->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
    last_send_sequence = send_sequence;
  }
  message->SetMessageSequence(send_sequence);

  message->SetTimestampSent(std::chrono::steady_clock::now());

  message_output_stream->SendMessage(message);

  if (message->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
    if (message->GetMessageType() == message_layer::MessageType::CONNECTION_RESET) {
      state = ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE;
    } else {
      state = ConnectionState::WAITING_FOR_ACKNOWLEDGE;
    }
  }
}

void Connection::SendAcknowledge(sequence_t acknowledged_msg_sequence) {
  auto ack_msg = message_layer::AcknowledgeMessage(acknowledged_msg_sequence);
  SendMessageNow(&ack_msg);
}

void Connection::Handle() {
  assert(state == ConnectionState::READY || state == ConnectionState::WAITING_FOR_ACKNOWLEDGE ||
         state == ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE);

  if (state == ConnectionState::READY) {
    if (!send_message_queue.empty()) {
      SendMessageNow(send_message_queue.front().get());
      send_message_queue.pop();
    }
  }
}

bool Connection::ConnectionClosed() const {
  return state == ConnectionState::CLOSED;
}
