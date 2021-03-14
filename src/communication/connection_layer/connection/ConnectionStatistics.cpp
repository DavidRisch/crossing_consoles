#include "ConnectionStatistics.h"

#include <iomanip>
#include <iostream>

using namespace communication::connection_layer;
using namespace communication;

const std::unordered_map<message_layer::MessageType, const std::string>
    ConnectionStatistics::map_message_type_to_string = {
        {message_layer::MessageType::CONNECTION_REQUEST, "CONNECTION_REQUEST"},
        {message_layer::MessageType::CONNECTION_RESPONSE, "CONNECTION_RESPONSE"},
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

ConnectionStatistics::ConnectionStatistics() = default;

void ConnectionStatistics::PrintMapStatistics() {
  int max = 30;
  std::cout << set_bold << "Connection Statistics \n" << reset;
  std::cout << std::setfill(' ') << "Message Type " << std::setw(18) << "  Sent " << std::setw(12) << " Received"
            << "\n";
  std::cout << std::setfill('-') << std::setw(line_length) << "\n";
  for (auto& entry : map_message_type_to_string) {
    int sent_entry = 0;
    int received_entry = 0;

    if (sent_message_map.find(entry.first) != sent_message_map.end()) {
      sent_entry = (sent_message_map.find(entry.first))->second;
    }

    if (received_message_map.find(entry.first) != received_message_map.end()) {
      received_entry = (received_message_map.find(entry.first))->second;
    }

    // calculate column width for better formatting
    auto diff = static_cast<int>(max - entry.second.length());

    std::cout << std::setfill(' ') << entry.second << std::setw(diff) << sent_entry << std::setw(8) << received_entry
              << "\n";
  }

  std::cout << std::setfill('-') << std::setw(45) << "\n";
  std::cout << std::setfill(' ') << "SUM" << std::setw(27) << sent_message_count << std::setw(8)
            << received_message_count << "\n";
}

void ConnectionStatistics::CalculatePackageLoss() {
  package_loss = (sent_message_count - sent_acknowledge_count) - received_acknowledge_count;

  if (package_loss != 0) {
    package_loss_percentage = static_cast<double>(package_loss) / (sent_message_count - sent_acknowledge_count) * 100;
  }
}

void ConnectionStatistics::PrintStatistics() {
  std::cout << "\n" << std::setfill('=') << std::setw(line_length) << "\n";

  PrintMapStatistics();
  CalculatePackageLoss();
  PrintPackageLoss();

  // TODO Use Message Meta Data and Acknowledges for time-related statistics

  std::cout << "\n" << std::setfill('=') << std::setw(line_length) << "\n";
}

void ConnectionStatistics::AddReceivedMessage(const message_layer::Message& message) {
  received_message_count++;
  if (message.GetMessageType() == message_layer::MessageType::ACKNOWLEDGE) {
    received_acknowledge_count++;
  }

  if (received_message_map.find(message.GetMessageType()) == received_message_map.end()) {
    received_message_map.insert_or_assign(message.GetMessageType(), 1);
  } else {
    auto value = received_message_map.find(message.GetMessageType());
    value->second++;
  }
}
void ConnectionStatistics::AddSentMessage(const message_layer::Message& message) {
  sent_message_count++;
  if (message.GetMessageType() == message_layer::MessageType::ACKNOWLEDGE) {
    sent_acknowledge_count++;
  }

  if (sent_message_map.find(message.GetMessageType()) == sent_message_map.end()) {
    sent_message_map.insert_or_assign(message.GetMessageType(), 1);
  } else {
    auto value = sent_message_map.find(message.GetMessageType());
    value->second++;
  }
}
void ConnectionStatistics::PrintPackageLoss() const {
  if (package_loss) {
    std::cout << set_red << "Package Loss: " << package_loss << " (" << package_loss_percentage << "%)\n" << reset;
  } else {
    std::cout << "Package Loss: " << package_loss << " (" << package_loss_percentage << "%)\n";
  }
}
