#include "fixtures/Statistics.h"

#include <gtest/gtest.h>

#include "fixtures/ConnectionManagers.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

TEST_F(Statistics, NoMessagesStatistics) {
  // no messages have been sent yet, statistics should be empty / zero
  create_server_and_client();

  ConnectionStatistics client_statistics =
      client_manager->GetStatisticsFromPartnerConnection(ProtocolDefinition::server_partner_id);
  ASSERT_EQ(client_statistics.GetSentMessageStatistics().total_count, 0);
  ASSERT_EQ(client_statistics.GetReceivedMessageStatistics().total_count, 0);
  ASSERT_FALSE(client_statistics.CalculateAverageResponseTime().has_value());
  ASSERT_EQ(client_statistics.CalculatePackageLoss().package_loss, 0);
  ASSERT_EQ(client_statistics.CalculatePackageLoss().package_loss_percentage, 0);
}

TEST_F(Statistics, SendMessagesStatistics) {
  int message_count = 10;
  create_server_and_client();
  send_and_check_messages(message_count);
  ConnectionStatistics client_statistics =
      client_manager->GetStatisticsFromPartnerConnection(ProtocolDefinition::server_partner_id);

  ConnectionStatistics::message_count_map_t sent_map = client_statistics.GetSentMessageStatistics().count_by_type;
  ASSERT_EQ(sent_map.at(MessageType::PAYLOAD), message_count);
  ASSERT_EQ(sent_map.at(MessageType::ACKNOWLEDGE), message_count);

  ConnectionStatistics::message_count_map_t received_map =
      client_statistics.GetReceivedMessageStatistics().count_by_type;
  ASSERT_EQ(received_map.at(MessageType::PAYLOAD), message_count);
  ASSERT_EQ(received_map.at(MessageType::ACKNOWLEDGE), message_count);

  ASSERT_EQ(client_statistics.CalculatePackageLoss().package_loss, 0);
  ASSERT_EQ(client_statistics.CalculatePackageLoss().package_loss_percentage, 0);
}

TEST_F(Statistics, PackageLossStatistics) {
  // Server becomes unreachable, package loss is 100%
  create_server_and_client();

  std::vector<uint8_t> payload;
  client_manager->SendDataToServer(payload);

  ConnectionStatistics client_statistics =
      client_manager->GetStatisticsFromPartnerConnection(ProtocolDefinition::server_partner_id);
  ConnectionStatistics::message_count_map_t sent_map = client_statistics.GetSentMessageStatistics().count_by_type;

  ASSERT_EQ(sent_map.at(MessageType::PAYLOAD), 1);
  ASSERT_EQ(client_statistics.CalculatePackageLoss().package_loss, 1);
  ASSERT_EQ(client_statistics.CalculatePackageLoss().package_loss_percentage, 100);
}
