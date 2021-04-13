#include "Connection.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <thread>
#include <utility>

#include "../../debug.h"
#include "../../message_layer/message/AcknowledgeMessage.h"
#include "../../message_layer/message/ConnectionRequestMessage.h"
#include "../../message_layer/message/ConnectionResponseMessage.h"
#include "../../message_layer/message/NotAcknowledgeMessage.h"
#include "../../message_layer/message_stream/MessageCoder.h"
#include "ConnectionManager.h"
#include "statistics/StatisticPrinter.h"

using namespace communication;
using namespace communication::connection_layer;

const char *communication::connection_layer::connection_state_to_string(ConnectionState value) {
  switch (value) {
    case ConnectionState::CLIENT_CONNECTION_START:
      return "CLIENT_CONNECTION_START";
    case ConnectionState::CLIENT_CONNECTION_REQUEST_SENT:
      return "CLIENT_CONNECTION_REQUEST_SENT";
    case ConnectionState::SERVER_WAITING_FOR_FIRST:
      return "SERVER_WAITING_FOR_FIRST";
    case ConnectionState::SERVER_CONNECTION_RESPONSE_SENT:
      return "SERVER_CONNECTION_RESPONSE_SENT";
    case ConnectionState::READY:
      return "READY";
    case ConnectionState::WAITING_FOR_ACKNOWLEDGE:
      return "WAITING_FOR_ACKNOWLEDGE";
    case ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE:
      return "WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE";
    case ConnectionState::CLOSED:
      return "CLOSED";
  }
  throw std::runtime_error("Invalid ConnectionState");
}

std::shared_ptr<message_layer::Message> Connection::ReceiveWithTimeout() {
  const auto start_time = std::chrono::steady_clock::now();

  while (std::chrono::steady_clock::now() - start_time <= timeout) {
    ResendIfNecessary();

    ReceiveAll();

    std::shared_ptr<message_layer::Message> message;
    if (!receive_message_queue.empty()) {
      message = receive_message_queue.front();
      receive_message_queue.pop();
    }

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
  return std::shared_ptr<Connection>(new Connection(std::move(message_input_stream), std::move(message_output_stream),
                                                    ConnectionState::CLIENT_CONNECTION_START,
                                                    1200,  // start with an arbitrarily chosen sequence
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

  ResendIfNecessary();

  switch (state) {
    case (ConnectionState::CLIENT_CONNECTION_START): {
      // step 1:
      SendMessageNow(std::make_shared<message_layer::ConnectionRequestMessage>(), false);

      state = ConnectionState::CLIENT_CONNECTION_REQUEST_SENT;
      break;
    }

    case (ConnectionState::CLIENT_CONNECTION_REQUEST_SENT): {
      DEBUG_CONNECTION_LOG(this, "Wait for step_2 start")
      auto step_2 = ReceiveWithTimeout();
      DEBUG_CONNECTION_LOG(this, "Wait for step_2 end")
      assert(step_2 != nullptr);
      if (step_2->GetMessageType() != message_layer::MessageType::CONNECTION_RESPONSE) {
        throw ConnectionCreationFailed();
      }
      unacknowledged_sent_message.clear();

      // step 3:
      SendMessageNow(std::make_shared<message_layer::AcknowledgeMessage>(step_2->GetMessageSequence()), false);

      state = ConnectionState::READY;
      break;
    }
    case (ConnectionState::SERVER_WAITING_FOR_FIRST): {
      DEBUG_CONNECTION_LOG(this, "Wait for step_1 start")
      auto step_1 = ReceiveWithTimeout();
      DEBUG_CONNECTION_LOG(this, "Wait for step_1 end")
      if (step_1->GetMessageType() != message_layer::MessageType::CONNECTION_REQUEST) {
        throw ConnectionCreationFailed();
      }

      // step 2:
      SendMessageNow(std::make_shared<message_layer::ConnectionResponseMessage>(), false);

      state = ConnectionState::SERVER_CONNECTION_RESPONSE_SENT;
      break;
    }
    case (ConnectionState::SERVER_CONNECTION_RESPONSE_SENT): {
      DEBUG_CONNECTION_LOG(this, "Wait for step_3 start")
      auto step_3 = ReceiveWithTimeout();
      DEBUG_CONNECTION_LOG(this, "Wait for step_3 end")
      if (step_3->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
        throw ConnectionCreationFailed();
      } else {
        auto *step_3_ack = dynamic_cast<message_layer::AcknowledgeMessage *>(step_3.get());
        if (step_3_ack->GetAcknowledgedMessageSequence() != last_send_sequence) {
          throw ConnectionCreationFailed();
        }
      }
      unacknowledged_sent_message.clear();
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

  ReceiveAll();

  std::shared_ptr<message_layer::Message> received_message;
  if (!receive_message_queue.empty()) {
    received_message = receive_message_queue.front();
    receive_message_queue.pop();
  }

  if (received_message != nullptr) {
    // set Metadata
    received_message->SetTimestampReceived(std::chrono::steady_clock::now());

    if (received_message->GetMessageType() == message_layer::MessageType::ACKNOWLEDGE) {
      if (!(state == ConnectionState::WAITING_FOR_ACKNOWLEDGE ||
            state == ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE)) {
        throw BadAcknowledgeException();
      }
      auto acknowledge_message = std::dynamic_pointer_cast<message_layer::AcknowledgeMessage>(received_message);
      auto acknowledged_sequence = acknowledge_message->GetAcknowledgedMessageSequence();
      if (acknowledged_sequence != last_send_sequence) {
        throw BadAcknowledgeException();
      }

      // Update unacknowledged_sent_message to remove the Messages which have definitely been received:

      auto it = std::find_if(unacknowledged_sent_message.begin(), unacknowledged_sent_message.end(),
                             [&acknowledged_sequence](const std::shared_ptr<message_layer::Message> &message) {
                               return message->GetMessageSequence() == acknowledged_sequence;
                             });

      if (it != unacknowledged_sent_message.end()) {
        (**it).SetTimestampReceived(std::chrono::steady_clock::now());
        statistics.AddSentAndAckMessage(*it);

        // remove all messages up to and including it
        unacknowledged_sent_message.erase(unacknowledged_sent_message.begin(), ++it);
      }

      if (state == ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE) {
        // acknowledge of ConnectionResetMessage has been received, close connection
        state = ConnectionState::CLOSED;
      } else {
        state = ConnectionState::READY;
      }
    } else if (received_message->GetMessageType() == message_layer::MessageType::NOT_ACKNOWLEDGE) {
      assert(false);  // NACKs are handled by TryReceive()
    } else {
      // Send acknowledge for every received message except for other (not-)acknowledges
      SendAcknowledge(received_message->GetMessageSequence());
    }
  }

  return received_message;
}

bool Connection::TryReceive() {
  std::shared_ptr<message_layer::Message> received_message;

  try {
    received_message = message_input_stream->ReceiveMessage(false);
  } catch (const message_layer::MessageCoder::DecodeFailedException &exception) {
    DEBUG_CONNECTION_LOG(this, "InvalidMessageException " << exception.what())

    auto message = std::make_shared<message_layer::NotAcknowledgeMessage>();
    SendMessageNow(message);

    return false;  // no further messages can be received
  }

  if (received_message == nullptr) {
    return false;  // no further messages can be received
  }

  // set Metadata
  received_message->SetTimestampReceived(std::chrono::steady_clock::now());

  if (state != ConnectionState::SERVER_CONNECTION_RESPONSE_SENT) {
    // connection establishment is not included in statistics
    statistics.AddReceivedMessage(received_message);
  }

  if (received_message->GetMessageType() != message_layer::MessageType::NOT_ACKNOWLEDGE) {
    if (last_received_sequence_counter.has_value()) {
      if (received_message->GetMessageSequence() != *last_received_sequence_counter + 1) {
        DEBUG_CONNECTION_LOG(this, "<~~ TryReceive: received type "
                                       << message_type_to_string(received_message->GetMessageType())
                                       << " seq: " << received_message->GetMessageSequence()
                                       << " expected: " << *last_received_sequence_counter + 1)
        return true;  // there might be more messages which can be received
      }
    }

    last_received_sequence_counter = received_message->GetMessageSequence();
  }

  DEBUG_CONNECTION_LOG(this, "<~~ TryReceive: received type "
                                 << message_type_to_string(received_message->GetMessageType())
                                 << " seq: " << received_message->GetMessageSequence())

  if (received_message->GetMessageType() == message_layer::MessageType::NOT_ACKNOWLEDGE) {
    DEBUG_CONNECTION_LOG(this, "received NACK")

    ResendLastMessages();
    return false;  // no further messages can be received
  }

  timestamp_last_change = std::chrono::steady_clock::now();
  receive_message_queue.push(received_message);

  return true;  // there might be more messages which can be received
}

void Connection::ReceiveAll() {
  while (TryReceive()) {
  }
}

Connection::Connection(std::shared_ptr<message_layer::MessageInputStream> message_input_stream,
                       std::shared_ptr<message_layer::MessageOutputStream> message_output_stream,
                       ConnectionState connection_state, sequence_t sequence_counter, timeout_t timeout)
    : state(connection_state)
    , timeout(timeout)
    , message_input_stream(std::move(message_input_stream))
    , message_output_stream(std::move(message_output_stream))
    , sequence_counter(sequence_counter) {
  resend_interval =
      std::chrono::duration_cast<std::chrono::milliseconds>(timeout / ProtocolDefinition::resend_denominator);
  timestamp_last_change = std::chrono::steady_clock::now();
}

sequence_t Connection::GenerateSequence() {
  sequence_t current_counter = sequence_counter;
  sequence_counter++;
  return current_counter;
}

void Connection::SendMessageNow(const std::shared_ptr<message_layer::Message> &message, bool expect_acknowledge) {
  if (message->GetMessageType() != message_layer::MessageType::NOT_ACKNOWLEDGE) {
    auto send_sequence = GenerateSequence();

    // Because ACKs are not acknowledged:
    if (message->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
      last_send_sequence = send_sequence;
    }
    message->SetMessageSequence(send_sequence);
  } else {
    message->SetMessageSequence(4444);  // the sequence of NACKs is never read, using dummy value
  }

  message->SetTimestampSent(std::chrono::steady_clock::now());

  DEBUG_CONNECTION_LOG(this, "~~~~> SendMessageNow: send type " << message_type_to_string(message->GetMessageType())
                                                                << " seq: " << message->GetMessageSequence())

  if (state != ConnectionState::CLIENT_CONNECTION_REQUEST_SENT) {
    // connection establishment is not included in statistics
    statistics.AddSentMessage(message);
  }

  message_output_stream->SendMessage(message.get());

  if (message->GetMessageType() != message_layer::MessageType::NOT_ACKNOWLEDGE) {
    unacknowledged_sent_message.push_back(message);
    timestamp_last_change = std::chrono::steady_clock::now();
    DEBUG_CONNECTION_LOG(this,
                         "Add to unacknowledged_sent_message now size " << (int)unacknowledged_sent_message.size())
  }

  if (expect_acknowledge && message->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE &&
      message->GetMessageType() != message_layer::MessageType::NOT_ACKNOWLEDGE) {
    if (message->GetMessageType() == message_layer::MessageType::CONNECTION_RESET) {
      state = ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE;
    } else {
      DEBUG_CONNECTION_LOG(this, "SendMessageNow State -> WAITING_FOR_ACKNOWLEDGE")
      state = ConnectionState::WAITING_FOR_ACKNOWLEDGE;
    }
  }
}

void Connection::SendAcknowledge(sequence_t acknowledged_msg_sequence) {
  auto ack_msg = std::make_shared<message_layer::AcknowledgeMessage>(acknowledged_msg_sequence);
  SendMessageNow(ack_msg);
}

void Connection::Handle() {
  assert(state == ConnectionState::READY || state == ConnectionState::WAITING_FOR_ACKNOWLEDGE ||
         state == ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE);

  DEBUG_CONNECTION_LOG(this, "Handle() current state: " << connection_state_to_string(state))

  ResendIfNecessary();

  if (state == ConnectionState::READY) {
    if (!send_message_queue.empty()) {
      SendMessageNow(send_message_queue.front());
      send_message_queue.pop();
    }
  }
}

const ConnectionStatistics &Connection::GetConnectionStatistics() const {
  return statistics;
}

bool Connection::ConnectionClosed() const {
  return state == ConnectionState::CLOSED;
}

bool Connection::IsUsable() const {
  return state == ConnectionState::READY || state == ConnectionState::WAITING_FOR_ACKNOWLEDGE;
}

void Connection::ResendIfNecessary() {
  auto current_time = std::chrono::steady_clock::now();
  if (current_time - timestamp_last_change >= resend_interval) {
    timestamp_last_change = current_time;
    ResendLastMessages();
  }
}

void Connection::ResendLastMessages() {
  if (!unacknowledged_sent_message.empty()) {
    for (const auto &item : unacknowledged_sent_message) {
      DEBUG_CONNECTION_LOG(this, "~~~~> Resend: send type " << message_type_to_string(item->GetMessageType())
                                                            << " seq: " << (int)item->GetMessageSequence())
      statistics.AddSentMessage(item);
      message_output_stream->SendMessage(item.get());
    }
  } else {
    DEBUG_CONNECTION_LOG(this, "Nothing to resend")
  }
}
