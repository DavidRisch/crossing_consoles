#include "fixtures/Statistics.h"

#include <gtest/gtest.h>

#include "fixtures/Connections.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

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
  // Test increase of uptime

  establish_connections();

  auto sleep_time = std::chrono::microseconds(1000);
  std::this_thread::sleep_for(sleep_time);

  auto uptime_server = server_connection->GetConnectionStatistics().CalculateUptime();
  auto uptime_client = client_connection->GetConnectionStatistics().CalculateUptime();

  // Check that uptime is in a reasonable range, half of the sleep_time is added as buffer
  EXPECT_GT(uptime_server.count(), 0);
  EXPECT_GT(uptime_client.count(), 0);

  std::this_thread::sleep_for(sleep_time);

  // Uptime increases
  EXPECT_LT(uptime_server, server_connection->GetConnectionStatistics().CalculateUptime());
  EXPECT_LT(uptime_client, client_connection->GetConnectionStatistics().CalculateUptime());
}
