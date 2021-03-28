#include "ConnectionStatistics.h"

#include <cassert>

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
    throw ConnectionStatistics::ContradictoryInputException();
  } else {
    sent_message_list.back().SetTimestampReceived(std::chrono::steady_clock::now());
  }
}

ConnectionStatistics::PackageLossData ConnectionStatistics::CalculatePackageLoss() const {
  // get number of sent acknowledges
  auto sent_acknowledge_entry = sent_message_statistics.count_by_type.find(message_layer::MessageType::ACKNOWLEDGE);
  message_count_t sent_acknowledge_count = 0;
  if (sent_acknowledge_entry != sent_message_statistics.count_by_type.end()) {
    sent_acknowledge_count = sent_acknowledge_entry->second;
  }

  // get number of received acknowledges
  auto received_acknowledge_entry =
      received_message_statistics.count_by_type.find(message_layer::MessageType::ACKNOWLEDGE);
  message_count_t received_acknowledge_count = 0;
  if (received_acknowledge_entry != received_message_statistics.count_by_type.end()) {
    received_acknowledge_count = received_acknowledge_entry->second;
  }

  ConnectionStatistics::PackageLossData package_loss_data;
  package_loss_data.package_loss =
      (sent_message_statistics.total_count - sent_acknowledge_count) - received_acknowledge_count;

  // a negative package loss is not possible
  assert(package_loss_data.package_loss >= 0);

  if (package_loss_data.package_loss != 0) {
    package_loss_data.package_loss_percentage = static_cast<double>(package_loss_data.package_loss) /
                                                (sent_message_statistics.total_count - sent_acknowledge_count) * 100;
  } else {
    package_loss_data.package_loss_percentage = 0;
  }
  return package_loss_data;
}

double ConnectionStatistics::CalculateAverageResponseTime() const {
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

double ConnectionStatistics::CalculateUptime() const {
  auto uptime =
      std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - connection_start_time);
  return static_cast<double>(uptime.count()) / 1000;
}

void ConnectionStatistics::UpdateStatisticData(MessageStatisticData& message_statistics,
                                               const message_layer::Message& message) {
  if (message.GetMessageType() == message_layer::MessageType::CONNECTION_RESPONSE ||
      message.GetMessageType() == message_layer::MessageType::CONNECTION_REQUEST) {
    return;
  }

  message_statistics.total_count++;

  // increment counter for MessageType of message
  auto message_type_it = message_statistics.count_by_type.find(message.GetMessageType());

  if (message_type_it == message_statistics.count_by_type.end()) {
    message_statistics.count_by_type.insert({message.GetMessageType(), 1});
  } else {
    message_type_it->second++;
  }
}

ConnectionStatistics::MessageStatisticData ConnectionStatistics::GetReceivedMessageStatistics() const {
  return received_message_statistics;
}
ConnectionStatistics::MessageStatisticData ConnectionStatistics::GetSentMessageStatistics() const {
  return sent_message_statistics;
}
