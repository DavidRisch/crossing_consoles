#ifndef CROSSING_CONSOLES_STATISTICS_H
#define CROSSING_CONSOLES_STATISTICS_H

#include <gtest/gtest.h>

#include "../../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "Connections.h"

class Statistics : public Connections {
 public:
  void establish_connections() {
    auto stream_pair = MockBidirectionalByteStream::CreatePair();
    auto& server_side = stream_pair.first;
    auto& client_side = stream_pair.second;

    auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
    auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

    auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
    auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

    make_connections(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                     server_message_output_stream);
  };

  static void check_statistics_balanced_by_type(const ConnectionStatistics& statistic_first,
                                                const ConnectionStatistics& statistic_second) {
    auto statistic_map_received_first = statistic_first.GetReceivedMessageStatistics().count_by_type;
    auto statistic_map_sent_first = statistic_first.GetSentMessageStatistics().count_by_type;
    auto statistic_map_sent_second = statistic_second.GetSentMessageStatistics().count_by_type;
    auto statistic_map_received_second = statistic_second.GetReceivedMessageStatistics().count_by_type;

    // All messages sent by first connection are received by second connection
    for (auto& first_it : statistic_map_sent_first) {
      auto second_it = statistic_map_received_second.find(first_it.first);

      if (second_it != statistic_map_received_second.end()) {
        ASSERT_EQ(first_it.second, second_it->second);
      }
    }

    // All messages sent by second connection are received by first connection
    for (auto& second_it : statistic_map_sent_second) {
      auto first_it = statistic_map_received_first.find(second_it.first);

      if (first_it != statistic_map_received_first.end()) {
        ASSERT_EQ(second_it.second, first_it->second);
      }
    }
  }

  static void check_statistics_balanced(const ConnectionStatistics& statistic_first,
                                        const ConnectionStatistics& statistic_second) {
    // Sent and received messages of server and client should complement each other

    ASSERT_EQ(statistic_first.GetReceivedMessageStatistics().total_count,
              statistic_second.GetSentMessageStatistics().total_count);
    ASSERT_EQ(statistic_first.GetSentMessageStatistics().total_count,
              statistic_second.GetReceivedMessageStatistics().total_count);

    check_statistics_balanced_by_type(statistic_first, statistic_second);

    ASSERT_EQ(statistic_first.CalculatePackageLoss().package_loss, 0);
    ASSERT_EQ(statistic_first.CalculatePackageLoss().package_loss_percentage, 0);

    ASSERT_EQ(statistic_second.CalculatePackageLoss().package_loss, 0);
    ASSERT_EQ(statistic_second.CalculatePackageLoss().package_loss_percentage, 0);
  }
};

#endif  // CROSSING_CONSOLES_STATISTICS_H
