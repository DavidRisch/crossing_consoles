#include "ConnectionStatistics.h"

#include <cassert>
#include <iomanip>
#include <iostream>

#include "Connection.h"

using namespace communication::connection_layer;
using namespace communication;

const std::unordered_map<message_layer::MessageType, const std::string>
    // ConnectionRequest and ConnectionResponse messages are ignored

    ConnectionStatistics::map_message_type_to_string = {
        {message_layer::MessageType::CONNECTION_RESET, "CONNECTION_RESET"},
        {message_layer::MessageType::ACKNOWLEDGE, "ACKNOWLEDGE"},
        {message_layer::MessageType::PAYLOAD, "PAYLOAD"},
        {message_layer::MessageType::KEEP_ALIVE, "KEEP_ALIVE"},
};

// console output parameters
const int line_length = 46;
const std::string esc_character = "\033";
const std::string set_red = esc_character + "[31m";  // sets output to be red
const std::string set_bold = esc_character + "[1m";  // sets output to be bold
const std::string reset = esc_character + "[0m";     // reset any console output changes

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

void ConnectionStatistics::PrintStatistics() {
  std::cout << "\n" << std::setfill('=') << std::setw(line_length) << "\n";

  PrintMapStatistics();
  std::cout << "\n";
  CalculatePackageLoss();
  PrintPackageLoss();
  PrintAverageResponseTime();
  PrintUptime();

  std::cout << "\n" << std::setfill('=') << std::setw(line_length) << "\n";
}

void ConnectionStatistics::PrintMapStatistics() {
  int max = 30;
  std::cout << set_bold << "Connection Statistics \n" << reset;
  std::cout << std::setfill(' ') << "Message Type " << std::setw(18) << "  Sent " << std::setw(12) << " Received"
            << "\n";
  std::cout << std::setfill('-') << std::setw(line_length) << "\n";
  for (auto& entry : map_message_type_to_string) {
    int sent_entry = 0;
    int received_entry = 0;

    if (sent_message_statistics.map.find(entry.first) != sent_message_statistics.map.end()) {
      sent_entry = (sent_message_statistics.map.find(entry.first))->second;
    }

    if (received_message_statistics.map.find(entry.first) != received_message_statistics.map.end()) {
      received_entry = (received_message_statistics.map.find(entry.first))->second;
    }

    // calculate column width for better formatting
    auto diff = static_cast<int>(max - entry.second.length());

    std::cout << std::setfill(' ') << entry.second << std::setw(diff) << sent_entry << std::setw(8) << received_entry
              << "\n";
  }

  std::cout << std::setfill('-') << std::setw(45) << "\n";
  std::cout << std::setfill(' ') << "SUM" << std::setw(27) << sent_message_statistics.count << std::setw(8)
            << received_message_statistics.count << "\n";
}

void ConnectionStatistics::CalculatePackageLoss() {
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

  package_loss = (sent_message_statistics.count - sent_acknowledge_count) - received_acknowledge_count;

  // a negative package loss is not possible
  assert(package_loss >= 0);

  if (package_loss != 0) {
    package_loss_percentage =
        static_cast<double>(package_loss) / (sent_message_statistics.count - sent_acknowledge_count) * 100;
  } else {
    package_loss_percentage = 0;
  }
}

void ConnectionStatistics::PrintPackageLoss() const {
  if (package_loss) {
    std::cout << set_red << "Package Loss: " << package_loss << " (" << std::setprecision(2) << std::fixed
              << package_loss_percentage << "%)\n"
              << reset;
  } else {
    std::cout << "Package Loss: " << package_loss << " (" << std::setprecision(2) << std::fixed
              << package_loss_percentage << "%)\n";
  }
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

void ConnectionStatistics::PrintAverageResponseTime() {
  std::cout << "Average Response Time: " << CalculateAverageResponseTime() << " ms\n";
}

double ConnectionStatistics::CalculateUptime() {
  auto uptime =
      std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - connection_start_time);
  return static_cast<double>(uptime.count()) / 1000;
}

void ConnectionStatistics::PrintUptime() {
  std::cout << "Uptime: " << CalculateUptime() << " ms\n";
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
