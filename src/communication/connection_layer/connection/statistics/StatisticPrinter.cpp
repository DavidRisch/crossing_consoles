#include "StatisticPrinter.h"

#include <iomanip>
#include <iostream>
#include <unordered_map>

using namespace communication;
using namespace communication::connection_layer;

const std::unordered_map<message_layer::MessageType, const std::string>
    // ConnectionRequest and ConnectionResponse messages are ignored

    StatisticPrinter::map_message_type_to_string = {
        {message_layer::MessageType::CONNECTION_RESET, "CONNECTION_RESET"},
        {message_layer::MessageType::ACKNOWLEDGE, "ACKNOWLEDGE"},
        {message_layer::MessageType::PAYLOAD, "PAYLOAD"},
        {message_layer::MessageType::KEEP_ALIVE, "KEEP_ALIVE"},
        {message_layer::MessageType::NOT_ACKNOWLEDGE, "NOT_ACKNOWLEDGE"},
};

// console output parameters
const int line_length = 60;
const std::string esc_character = "\033";
const std::string set_red = esc_character + "[31m";  // sets output to be red
const std::string set_bold = esc_character + "[1m";  // sets output to be bold
const std::string reset = esc_character + "[0m";     // reset any console output changes

void StatisticPrinter::PrintMapStatistics(
    const ConnectionStatistics::MessageStatisticData& sent_message_statistics,
    const ConnectionStatistics::MessageStatisticData& received_message_statistics,
    const ConnectionStatistics::MessageStatisticData& sent_and_ack_message_statistics) {
  const int column_width = 16;
  const int column_count_width = 10;

  // Print headlines
  std::cout << set_bold << "Connection Statistics \n\n" << reset;
  std::cout << "Message Type ";
  std::cout << std::setw(column_width) << "  Sent ";
  std::cout << std::setw(column_width) << " Received";
  std::cout << std::setw(column_width) << " Sent & Ack"
            << "\n";

  std::cout << std::setfill('-') << std::setw(line_length) << ""
            << "\n"
            << std::setfill(' ');

  // Print message count sorted by message type
  for (auto& entry : map_message_type_to_string) {
    int sent_entry = 0;
    int received_entry = 0;
    int sent_and_ack_entry = 0;

    if (sent_message_statistics.count_by_type.find(entry.first) != sent_message_statistics.count_by_type.end()) {
      sent_entry = (sent_message_statistics.count_by_type.find(entry.first))->second;
    }

    if (sent_and_ack_message_statistics.count_by_type.find(entry.first) !=
        sent_and_ack_message_statistics.count_by_type.end()) {
      sent_and_ack_entry = (sent_and_ack_message_statistics.count_by_type.find(entry.first))->second;
    }

    if (received_message_statistics.count_by_type.find(entry.first) !=
        received_message_statistics.count_by_type.end()) {
      received_entry = (received_message_statistics.count_by_type.find(entry.first))->second;
    }

    std::cout << std::setw(column_width) << std::left << entry.second << std::right;
    std::cout << std::setw(column_count_width) << sent_entry;
    std::cout << std::setw(column_width) << received_entry;
    std::cout << std::setw(column_width) << sent_and_ack_entry << "\n";
  }

  // Print SUM
  std::cout << std::setfill('-') << std::setw(line_length) << ""
            << "\n"
            << std::setfill(' ');
  std::cout << std::setw(column_width) << std::left << "SUM" << std::right;
  std::cout << std::setw(column_count_width) << sent_message_statistics.total_count;
  std::cout << std::setw(column_width) << received_message_statistics.total_count;
  std::cout << std::setw(column_width) << sent_and_ack_message_statistics.total_count << "\n";
}

void StatisticPrinter::PrintPackageLoss(ConnectionStatistics::PackageLossData data) {
  if (data.package_loss) {
    std::cout << set_red << "Package Loss: " << data.package_loss << " (" << std::setprecision(2) << std::fixed
              << data.package_loss_percentage << "%)\n"
              << reset;
  } else {
    std::cout << "Package Loss: " << data.package_loss << " (" << std::setprecision(2) << std::fixed
              << data.package_loss_percentage << "%)\n";
  }
}

void StatisticPrinter::PrintAverageResponseTime(const std::optional<std::chrono::microseconds> response_time) {
  if (!response_time.has_value()) {
    std::cout << "Average response time not available, no messages sent. \n";
  } else {
    std::cout << "Average Response Time: " << response_time->count() / 1000.0 << " ms\n";
  }
}

void StatisticPrinter::PrintUptime(const std::chrono::microseconds uptime) {
  std::cout << "Uptime: " << uptime.count() / 1000.0 << " ms\n";
}

void StatisticPrinter::PrintStatistics(const ConnectionStatistics& statistics) {
  std::cout << "\n"
            << std::setfill('=') << std::setw(line_length) << ""
            << "\n"
            << std::setfill(' ');

  PrintMapStatistics(statistics.GetSentMessageStatistics(), statistics.GetReceivedMessageStatistics(),
                     statistics.GetSentAndAcknowledgedMessageStatistics());
  std::cout << "\n";
  PrintPackageLoss(statistics.CalculatePackageLoss());
  PrintAverageResponseTime(statistics.GetAverageResponseTime());
  PrintUptime(statistics.CalculateUptime());

  std::cout << "\n"
            << std::setfill('=') << std::setw(line_length) << ""
            << "\n"
            << std::setfill(' ');
}
