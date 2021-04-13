#include "ConnectionStatistics.h"

#include <cassert>

using namespace communication::connection_layer;
using namespace communication;

ConnectionStatistics::ConnectionStatistics() {
  connection_start_time = std::chrono::steady_clock::now();
}

void ConnectionStatistics::AddReceivedMessage(const std::shared_ptr<message_layer::Message>& message) {
  UpdateStatisticData(received_message_statistics, message);
}

void ConnectionStatistics::AddSentMessage(const std::shared_ptr<message_layer::Message>& message) {
  UpdateStatisticData(sent_message_statistics, message);
}

void ConnectionStatistics::AddSentAndAckMessage(const std::shared_ptr<message_layer::Message>& message) {
  UpdateStatisticData(sent_and_ack_message_statistics, message);
  UpdateAverageResponseTimeSum(message);
}

ConnectionStatistics::PackageLossData ConnectionStatistics::CalculatePackageLoss() const {
  // get number of sent acknowledges
  auto sent_acknowledge_entry = sent_message_statistics.count_by_type.find(message_layer::MessageType::ACKNOWLEDGE);
  message_count_t sent_acknowledge_count = 0;
  if (sent_acknowledge_entry != sent_message_statistics.count_by_type.end()) {
    sent_acknowledge_count = sent_acknowledge_entry->second;
  }

  auto sent_messages_without_acknowledges = sent_message_statistics.total_count - sent_acknowledge_count;

  ConnectionStatistics::PackageLossData package_loss_data;
  package_loss_data.package_loss = (sent_messages_without_acknowledges - sent_and_ack_message_statistics.total_count);

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

std::optional<std::chrono::microseconds> ConnectionStatistics::GetAverageResponseTime() const {
  if (sent_and_ack_message_statistics.total_count == 0) {
    return std::optional<std::chrono::microseconds>();
  }

  return response_time_sum / sent_and_ack_message_statistics.total_count;
}

std::chrono::microseconds ConnectionStatistics::CalculateUptime() const {
  return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() -
                                                               connection_start_time);
}

void ConnectionStatistics::UpdateStatisticData(MessageStatisticData& message_statistics,
                                               const std::shared_ptr<message_layer::Message>& message) {
  if (message->GetMessageType() == message_layer::MessageType::CONNECTION_RESPONSE ||
      message->GetMessageType() == message_layer::MessageType::CONNECTION_REQUEST) {
    return;
  }

  message_statistics.total_count++;

  // increment counter for MessageType of message
  auto message_type_it = message_statistics.count_by_type.find(message->GetMessageType());

  if (message_type_it == message_statistics.count_by_type.end()) {
    message_statistics.count_by_type.insert_or_assign(message->GetMessageType(), 1);

  } else {
    message_statistics.count_by_type.find(message_type_it->first)->second++;
  }
}

ConnectionStatistics::MessageStatisticData ConnectionStatistics::GetReceivedMessageStatistics() const {
  return received_message_statistics;
}

ConnectionStatistics::MessageStatisticData ConnectionStatistics::GetSentMessageStatistics() const {
  return sent_message_statistics;
}

ConnectionStatistics::MessageStatisticData ConnectionStatistics::GetSentAndAcknowledgedMessageStatistics() const {
  return sent_and_ack_message_statistics;
}

void ConnectionStatistics::UpdateAverageResponseTimeSum(const std::shared_ptr<message_layer::Message>& message) {
  auto meta_data = message->GetMessageMetaData();

  if (meta_data.GetTimestampReceived().has_value()) {
    auto interval = std::chrono::duration_cast<std::chrono::microseconds>(*meta_data.GetTimestampReceived() -
                                                                          *meta_data.GetTimestampSent());
    response_time_sum += interval;
  }
}
