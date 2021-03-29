#include "Connection.h"

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
      DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") Wait for step_2 start\n")
      auto step_2 = ReceiveWithTimeout();
      DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") Wait for step_2 end\n")
      assert(step_2 != nullptr);
      if (step_2->GetMessageType() != message_layer::MessageType::CONNECTION_RESPONSE) {
        throw ConnectionCreationFailed();
      }

      // step 3:
      SendMessageNow(std::make_shared<message_layer::AcknowledgeMessage>(step_2->GetMessageSequence()), false);

      state = ConnectionState::READY;
      break;
    }
    case (ConnectionState::SERVER_WAITING_FOR_FIRST): {
      DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") Wait for step_1 start\n")
      auto step_1 = ReceiveWithTimeout();
      DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") Wait for step_1 end\n")
      if (step_1->GetMessageType() != message_layer::MessageType::CONNECTION_REQUEST) {
        throw ConnectionCreationFailed();
      }

      // step 2:
      SendMessageNow(std::make_shared<message_layer::ConnectionResponseMessage>(), false);

      state = ConnectionState::SERVER_CONNECTION_RESPONSE_SENT;
      break;
    }
    case (ConnectionState::SERVER_CONNECTION_RESPONSE_SENT): {
      DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") Wait for step_3 start\n")
      auto step_3 = ReceiveWithTimeout();
      DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") Wait for step_3 end\n")
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
        statistics.AddSentAndAckMessage(**it);
        unacknowledged_sent_message.erase(unacknowledged_sent_message.begin(), it);
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
    DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") InvalidMessageException " << exception.what() << "\n")

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
    statistics.AddReceivedMessage(*received_message);
  }

  if (received_message->GetMessageType() != message_layer::MessageType::NOT_ACKNOWLEDGE) {
    if (last_received_sequence_counter.has_value()) {
      if (received_message->GetMessageSequence() != *last_received_sequence_counter + 1) {
        DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") <~~ TryReceive: received type " << std::dec
                                         << (int)received_message->GetMessageType()
                                         << "   \t seq: " << received_message->GetMessageSequence()
                                         << " expected: " << *last_received_sequence_counter + 1 << "\n")
        return true;  // there might be more messages which can be received
      }
    }

    last_received_sequence_counter = received_message->GetMessageSequence();
  }

  DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") <~~ TryReceive: received type " << std::dec
                                   << (int)received_message->GetMessageType()
                                   << "   \t seq: " << received_message->GetMessageSequence() << "\n")

  if (received_message->GetMessageType() == message_layer::MessageType::NOT_ACKNOWLEDGE) {
    DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") received NACK\n")

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
    , sequence_counter(sequence_counter)
    , timeout(timeout)
    , message_input_stream(std::move(message_input_stream))
    , message_output_stream(std::move(message_output_stream)) {
  resend_interval =
      std::chrono::duration_cast<std::chrono::milliseconds>(timeout / ProtocolDefinition::resend_numerator);
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

  DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") ~~~~> SendMessageNow: send type " << std::dec
                                   << (int)message->GetMessageType() << "  \t seq: " << message->GetMessageSequence()
                                   << "\n")

  if (state != ConnectionState::CLIENT_CONNECTION_REQUEST_SENT) {
    // connection establishment is not included in statistics
    statistics.AddSentMessage(*message);
  }

  message_output_stream->SendMessage(message.get());

  if (message->GetMessageType() != message_layer::MessageType::NOT_ACKNOWLEDGE) {
    unacknowledged_sent_message.push_back(message);
    timestamp_last_change = std::chrono::steady_clock::now();
    DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") Add to unacknowledged_sent_message now size "
                                     << (int)unacknowledged_sent_message.size() << "\n")
  }

  if (expect_acknowledge && message->GetMessageType() != message_layer::MessageType::ACKNOWLEDGE &&
      message->GetMessageType() != message_layer::MessageType::NOT_ACKNOWLEDGE) {
    if (message->GetMessageType() == message_layer::MessageType::CONNECTION_RESET) {
      state = ConnectionState::WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE;
    } else {
      DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") SendMessageNow State -> WAITING_FOR_ACKNOWLEDGE "
                                       << "\n")
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

  DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") Handle. current state " << (int)state << "\n")

  ResendIfNecessary();

  if (state == ConnectionState::READY) {
    if (!send_message_queue.empty()) {
      SendMessageNow(send_message_queue.front());
      send_message_queue.pop();
    }
  }
}

ConnectionStatistics Connection::GetConnectionStatistics() const {
  return statistics;
}

bool Connection::ConnectionClosed() const {
  return state == ConnectionState::CLOSED;
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
    DEBUG_CONNECTION_LAYER(std::cout << "(" << this << ") \n")
    for (const auto &item : unacknowledged_sent_message) {
      DEBUG_CONNECTION_LAYER(std::cout << "          ~~~~> Resend: send type " << std::dec
                                       << (int)item->GetMessageType() << "  \t seq: " << (int)item->GetMessageSequence()
                                       << "\n")
      statistics.AddSentMessage(*item);
      message_output_stream->SendMessage(item.get());
    }
  } else {
    DEBUG_CONNECTION_LAYER(std::cout << "          Nothing to resend...\n")
  }
}
