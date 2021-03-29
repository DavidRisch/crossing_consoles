#include "fixtures/Statistics.h"

#include <gtest/gtest.h>

#include "fixtures/Connections.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

void check_statistics_balanced_by_type(const ConnectionStatistics& statistic_first,
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
    } else {
      ASSERT_FALSE(true);
    }
  }

  // All messages sent by second connection are received by first connection
  for (auto& second_it : statistic_map_sent_second) {
    auto first_it = statistic_map_received_first.find(second_it.first);

    if (first_it != statistic_map_received_first.end()) {
      ASSERT_EQ(second_it.second, first_it->second);
    } else {
      ASSERT_FALSE(true);
    }
  }
}

void check_statistics_balanced(const ConnectionStatistics& statistic_first,
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

TEST_F(Statistics, NoMessages) {
  // no messages have been sent yet, statistics should be empty / zero

  establish_connections();

  auto statistics_server = server_connection->GetConnectionStatistics();
  auto statistics_client = client_connection->GetConnectionStatistics();

  ASSERT_EQ(statistics_server.GetReceivedMessageStatistics().total_count, 0);
  ASSERT_EQ(statistics_server.GetSentMessageStatistics().total_count, 0);
  ASSERT_EQ(statistics_server.GetSentAndAcknowledgedMessageStatistics().total_count, 0);

  check_statistics_balanced(statistics_server, statistics_client);
  ASSERT_EQ(statistics_client.GetSentAndAcknowledgedMessageStatistics().total_count, 0);
}

TEST_F(Statistics, Simple) {
  // Send all types of Messages from server to client without any unacknowledged packages

  establish_connections();

  // clients sends two Messages of Type KeepAlive, server sends two messages of type Payload
  test_connections();
  test_connections();

  // server sends message of type CloseConnection
  close_connection(server_connection, client_connection);

  auto statistic_server = server_connection->GetConnectionStatistics();
  auto statistic_client = client_connection->GetConnectionStatistics();

  ASSERT_TRUE(statistic_server.CalculateAverageResponseTime().has_value());
  ASSERT_TRUE(statistic_client.CalculateAverageResponseTime().has_value());

  ASSERT_EQ(statistic_client.GetSentAndAcknowledgedMessageStatistics().total_count, 2);
  ASSERT_EQ(statistic_server.GetSentAndAcknowledgedMessageStatistics().total_count, 3);

  check_statistics_balanced(statistic_server, statistic_client);
}

TEST_F(Statistics, ManyMessages) {
  // Send 100 Messages from server to client and vice versa

  establish_connections();

  int count = 100;
  for (int i = 0; i < count; i++) {
    // clients sends one message of type KeepAlive, server sends one message of type Payload on each call
    test_connections();
  }

  // client sends message of type CloseConnection
  close_connection(client_connection, server_connection);

  auto statistic_server = server_connection->GetConnectionStatistics();
  auto statistic_client = client_connection->GetConnectionStatistics();

  ASSERT_TRUE(statistic_server.CalculateAverageResponseTime().has_value());
  ASSERT_TRUE(statistic_client.CalculateAverageResponseTime().has_value());

  // client has one sent & acknowledged message more than server because of connection reset
  ASSERT_EQ(statistic_client.GetSentAndAcknowledgedMessageStatistics().total_count, count + 1);
  ASSERT_EQ(statistic_server.GetSentAndAcknowledgedMessageStatistics().total_count, count);

  check_statistics_balanced(statistic_server, statistic_client);
}

TEST_F(Statistics, Uptime) {
  // Test increase and range of uptime

  establish_connections();

  auto sleep_time = std::chrono::microseconds(1000);
  std::this_thread::sleep_for(sleep_time);

  auto uptime_server = server_connection->GetConnectionStatistics().CalculateUptime();
  auto uptime_client = client_connection->GetConnectionStatistics().CalculateUptime();

  // Check that uptime is in a reasonable range, half of the sleep_time is added as buffer
  ASSERT_TRUE(uptime_server.count() > 0 && uptime_server < 1.5 * sleep_time);
  ASSERT_TRUE(uptime_client.count() > 0 && uptime_client < 1.5 * sleep_time);

  std::this_thread::sleep_for(sleep_time);

  // Uptime increases
  ASSERT_TRUE(uptime_server < server_connection->GetConnectionStatistics().CalculateUptime());
  ASSERT_TRUE(uptime_client < client_connection->GetConnectionStatistics().CalculateUptime());
}
