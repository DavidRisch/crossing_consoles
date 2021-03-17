#include "ConnectionStatistics.h"

#include <cassert>
#include <iostream>

#include "../Connection.h"

using namespace communication::connection_layer;
using namespace communication;

ConnectionStatistics::ConnectionStatistics() {
  connection_start_time = std::chrono::steady_clock::now();
}

void ConnectionStatistics::AddReceivedMessage(const message_layer::Message& message) {
  UpdateStatisticData(received_message_statistics, message);
}

void ConnectionStatistics::AddSentMessage(const message_layer::Message& message) {
  UpdateStatisticData(sent_message_statistics, message);

  // Sent acknowledges and reset messages can not be used to measure response time
  if (message.GetMessageType() != message_layer::MessageType::ACKNOWLEDGE) {
    sent_message_list.push_back(message);
  }
}

void ConnectionStatistics::SetReceivedTimestampForSentMessage(ProtocolDefinition::sequence_t sequence) {
  assert(!sent_message_list.empty());
  if (sent_message_list.back().GetMessageSequence() != sequence) {
    throw Connection::BadAcknowledgeException();
  } else {
    sent_message_list.back().SetTimestampReceived(std::chrono::steady_clock::now());
  }
}

ConnectionStatistics::PackageLossData ConnectionStatistics::CalculatePackageLoss() {
  // get number of sent acknowledges
  auto sent_acknowledge_entry = sent_message_statistics.map.find(message_layer::MessageType::ACKNOWLEDGE);
  message_count_t sent_acknowledge_count = 0;
  if (sent_acknowledge_entry != sent_message_statistics.map.end()) {
    sent_acknowledge_count = sent_acknowledge_entry->second;
  }

  // get number of received acknowledges
  auto received_acknowledge_entry = received_message_statistics.map.find(message_layer::MessageType::ACKNOWLEDGE);
  message_count_t received_acknowledge_count = 0;
  if (received_acknowledge_entry != received_message_statistics.map.end()) {
    received_acknowledge_count = received_acknowledge_entry->second;
  }

  ConnectionStatistics::PackageLossData package_loss_data;
  package_loss_data.package_loss =
      (sent_message_statistics.count - sent_acknowledge_count) - received_acknowledge_count;

  // a negative package loss is not possible
  assert(package_loss_data.package_loss >= 0);

  if (package_loss_data.package_loss != 0) {
    package_loss_data.package_loss_percentage = static_cast<double>(package_loss_data.package_loss) /
                                                (sent_message_statistics.count - sent_acknowledge_count) * 100;
  } else {
    package_loss_data.package_loss_percentage = 0;
  }
  return package_loss_data;
}

double ConnectionStatistics::CalculateAverageResponseTime() {
  if (sent_message_list.empty()) {
    return 0;
  }
  std::chrono::duration<int64_t, std::micro> sum = std::chrono::microseconds(0);
  for (auto& message : sent_message_list) {
    auto meta_data = message.GetMessageMetaData();
    if (meta_data.GetTimestampReceived().has_value()) {
      sum += std::chrono::duration_cast<std::chrono::microseconds>(*meta_data.GetTimestampReceived() -
                                                                   *meta_data.GetTimestampSent());
    }
  }
  return (static_cast<double>(sum.count()) / sent_message_list.size()) / 1000;
}

double ConnectionStatistics::CalculateUptime() {
  auto uptime =
      std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - connection_start_time);
  return static_cast<double>(uptime.count()) / 1000;
}

void ConnectionStatistics::UpdateStatisticData(MessageStatisticData& message_statistics,
                                               const message_layer::Message& message) {
  assert(message.GetMessageType() != message_layer::MessageType::CONNECTION_RESPONSE);
  assert(message.GetMessageType() != message_layer::MessageType::CONNECTION_REQUEST);

  message_statistics.count++;

  // increment counter for MessageType of message
  if (message_statistics.map.find(message.GetMessageType()) == message_statistics.map.end()) {
    message_statistics.map.insert_or_assign(message.GetMessageType(), 1);
  } else {
    auto value = message_statistics.map.find(message.GetMessageType());
    value->second++;
  }
}

ConnectionStatistics::MessageStatisticData ConnectionStatistics::GetReceivedMessageStatistics() {
  return received_message_statistics;
}
ConnectionStatistics::MessageStatisticData ConnectionStatistics::GetSentMessageStatistics() {
  return sent_message_statistics;
}
