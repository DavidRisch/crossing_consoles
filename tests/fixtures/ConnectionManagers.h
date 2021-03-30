#ifndef CROSSING_CONSOLES_CONNECTION_MANAGERS_H
#define CROSSING_CONSOLES_CONNECTION_MANAGERS_H

#include <gtest/gtest.h>

#include <thread>

#include "../../src/communication/ProtocolDefinition.h"
#include "../../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../../src/communication/connection_layer/connection/ClientSideConnectionManager.h"
#include "../../src/communication/connection_layer/connection/ServerSideConnectionManager.h"
#include "../../src/communication/connection_layer/event/PayloadEvent.h"
#include "../utils/detect_debugger.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

class ConnectionManagers : public ::testing::Test {
 public:
  ConnectionManagers()
      : server_connection_simulator_provider(PerfectConnectionSimulatorProvider::instance)
      , client_connection_simulator_provider(PerfectConnectionSimulatorProvider::instance){};

  std::shared_ptr<ServerSideConnectionManager> server_manager;
  std::shared_ptr<ClientSideConnectionManager> client_manager;
  std::shared_ptr<ClientSideConnectionManager> second_client_manager;

  ProtocolDefinition::partner_id_t client_id{};         // from server perspective
  ProtocolDefinition::partner_id_t second_client_id{};  // from server perspective
  ProtocolDefinition::partner_id_t server_id{};         // from client perspective

  void create_server_and_client(ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout) {
    server_manager = ServerSideConnectionManager::CreateServerSide(timeout, server_connection_simulator_provider);
    std::thread server_thread([this] {
      int counter = 100;
      while (counter > 0 && server_manager->ConnectionCount() == 0) {
        server_manager->HandleConnections();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        counter--;
      }
      ASSERT_GT(counter, 0);
      assert(counter > 0);
    });

    client_manager = ClientSideConnectionManager::CreateClientSide(timeout, client_connection_simulator_provider);

    while (!server_manager->HasConnections()) {
      client_manager->HandleConnections();
      std::this_thread::sleep_for(std::chrono::microseconds(10));
    }

    server_thread.join();

    ASSERT_TRUE(server_manager->ConnectionCount() != 0);

    {
      auto event = server_manager->PopAndGetOldestEvent();
      ASSERT_NE(event, nullptr);
      ASSERT_EQ(event->GetType(), EventType::CONNECT);
      client_id = event->GetPartnerId();
      EXPECT_NE(client_id, ProtocolDefinition::server_partner_id);
    }
    ASSERT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);

    {
      auto event = client_manager->PopAndGetOldestEvent();
      ASSERT_NE(event, nullptr);
      ASSERT_EQ(event->GetType(), EventType::CONNECT);
      server_id = event->GetPartnerId();
      EXPECT_EQ(server_id, ProtocolDefinition::server_partner_id);
    }
    ASSERT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
  }

  void create_second_client() {
    assert(server_manager->ConnectionCount() == 1);

    std::thread server_thread([this] {
      int counter = 100;
      while (counter > 0 && server_manager->ConnectionCount() == 1) {
        server_manager->HandleConnections();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        counter--;
      }
      ASSERT_GT(counter, 0);
    });

    second_client_manager = ClientSideConnectionManager::CreateClientSide(ProtocolDefinition::timeout,
                                                                          client_connection_simulator_provider);
    server_thread.join();

    {
      auto event = server_manager->PopAndGetOldestEvent();
      ASSERT_NE(event, nullptr);
      ASSERT_EQ(event->GetType(), EventType::CONNECT);
      second_client_id = event->GetPartnerId();
      EXPECT_NE(second_client_id, ProtocolDefinition::server_partner_id);
    }
    ASSERT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);

    {
      auto event = second_client_manager->PopAndGetOldestEvent();
      ASSERT_NE(event, nullptr);
      ASSERT_EQ(event->GetType(), EventType::CONNECT);
      auto new_server_id = event->GetPartnerId();
      EXPECT_EQ(server_id, new_server_id);
    }
    ASSERT_EQ(second_client_manager->PopAndGetOldestEvent(), nullptr);
  }

  static void assert_payload_received(const std::shared_ptr<Event> &event, const std::vector<uint8_t> &payload) {
    if (event == nullptr) {
      assert(false);
    }
    ASSERT_NE(event, nullptr);
    ASSERT_EQ(event->GetType(), EventType::PAYLOAD);
    auto payload_event = std::dynamic_pointer_cast<PayloadEvent>(event);
    ASSERT_EQ(payload_event->GetPayload().size(), payload.size());
    for (size_t i = 0; i < payload.size(); ++i) {
      EXPECT_EQ(payload_event->GetPayload().at(i), payload.at(i));
    }
  }

  void send_and_check_messages(int count) {
    for (int i = 0; i < count; ++i) {
      std::vector<uint8_t> payload_server_to_client{1, 2, 3, static_cast<uint8_t>(i)};
      std::vector<uint8_t> payload_client_to_server{5, 6, 7, static_cast<uint8_t>(i)};

      server_manager->SendDataToConnection(client_id, payload_server_to_client);
      client_manager->SendDataToServer(payload_client_to_server);
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      int counter;
      for (counter = 0; counter < 10000 && (!server_manager->HasEvent() || !client_manager->HasEvent()); ++counter) {
        server_manager->HandleConnections();
        client_manager->HandleConnections();
        std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      ASSERT_LT(counter, 10000);

      assert_payload_received(server_manager->PopAndGetOldestEvent(), payload_client_to_server);
      assert_payload_received(client_manager->PopAndGetOldestEvent(), payload_server_to_client);

      // all events should have been processed by now
      EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
      EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
    }
  }

  void destroy() {
    server_manager.reset();
    client_manager.reset();
    second_client_manager.reset();
  }

 protected:
  std::shared_ptr<byte_layer::IConnectionSimulatorProvider> server_connection_simulator_provider;
  std::shared_ptr<byte_layer::IConnectionSimulatorProvider> client_connection_simulator_provider;
};

#endif
