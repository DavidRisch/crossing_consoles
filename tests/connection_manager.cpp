#include <gtest/gtest.h>

#include <thread>

#include "../src/communication/ProtocolDefinition.h"
#include "../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../src/communication/connection_layer/connection/ClientSideConnectionManager.h"
#include "../src/communication/connection_layer/connection/ServerSideConnectionManager.h"
#include "../src/communication/connection_layer/event/PayloadEvent.h"
#include "../src/communication/message_layer/message/ConnectionResetMessage.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

class ConnectionManagers : public ::testing::Test {
 public:
  ConnectionManagers() = default;

  std::shared_ptr<ServerSideConnectionManager> server_manager;
  std::shared_ptr<ClientSideConnectionManager> client_manager;
  std::shared_ptr<ClientSideConnectionManager> second_client_manager;

  ProtocolDefinition::address_t client_id{};         // from server perspective
  ProtocolDefinition::address_t second_client_id{};  // from server perspective
  ProtocolDefinition::address_t server_id{};         // from client perspective

  void create_server_and_client() {
    server_manager = ServerSideConnectionManager::CreateServerSide();
    std::thread server_thread([this] {
      int counter = 10;
      while (counter > 0 && server_manager->ConnectionCount() == 0) {
        server_manager->HandleConnections();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        counter--;
      }
    });

    client_manager = ClientSideConnectionManager::CreateClientSide();
    server_thread.join();

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
      int counter = 10;
      while (counter > 0 && server_manager->ConnectionCount() == 1) {
        server_manager->HandleConnections();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        counter--;
      }
    });

    second_client_manager = ClientSideConnectionManager::CreateClientSide();
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
};

TEST_F(ConnectionManagers, ConnectClient) {
  create_server_and_client();
}

TEST_F(ConnectionManagers, ServerTimeout) {
  // Server becomes unreachable

  std::shared_ptr<ServerSideConnectionManager> server_manager =
      ServerSideConnectionManager::CreateServerSide(std::chrono::milliseconds(10));
  std::thread server_thread([&server_manager] {
    int counter = 10;
    while (counter > 0 && !server_manager->ConnectionCount()) {
      server_manager->HandleConnections();
      std::this_thread::sleep_for(std::chrono::microseconds(10));
      counter--;
    }
  });

  // set low timeout
  auto client_manager = ClientSideConnectionManager::CreateClientSide(std::chrono::milliseconds(10));
  server_thread.join();
  client_manager->HandleConnections();

  // client is successfully connected
  ASSERT_EQ(client_manager->PopAndGetOldestEvent()->GetType(), EventType::CONNECT);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // client should be disconnected from timeout
  client_manager->HandleConnections();
  ASSERT_EQ(client_manager->PopAndGetOldestEvent()->GetType(), EventType::DISCONNECT);
}

TEST_F(ConnectionManagers, ClientTimeout) {
  // Client becomes unreachable

  std::shared_ptr<ServerSideConnectionManager> server_manager =
      ServerSideConnectionManager::CreateServerSide(std::chrono::milliseconds(10));
  std::thread server_thread([&server_manager] {
    int counter = 10;
    while (!server_manager->ConnectionCount() || counter > 0) {
      server_manager->HandleConnections();
      std::this_thread::sleep_for(std::chrono::microseconds(10));
      counter--;
    }

    // Server is connected to client
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    auto event = server_manager->PopAndGetOldestEvent();
    if (event) {
      EXPECT_EQ(event->GetType(), EventType::CONNECT);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Client disconnects
    server_manager->HandleConnections();
    event = server_manager->PopAndGetOldestEvent();
    if (event) {
      ASSERT_EQ(event->GetType(), EventType::DISCONNECT);
      ASSERT_FALSE(server_manager->ConnectionCount());
    }
  });
  auto client_manager = ClientSideConnectionManager::CreateClientSide();
  server_thread.join();
}

TEST_F(ConnectionManagers, UnknownPartnerException) {
  create_server_and_client();
  std::vector<uint8_t> payload;
  EXPECT_THROW(server_manager->SendDataToConnection(client_id + 1, payload),
               ConnectionManager::UnknownPartnerException);
  EXPECT_THROW(client_manager->SendDataToConnection(server_id + 1, payload),
               ConnectionManager::UnknownPartnerException);
}

void assert_payload_received(const std::shared_ptr<Event> &event, const std::vector<uint8_t> &payload) {
  ASSERT_NE(event, nullptr);
  ASSERT_EQ(event->GetType(), EventType::PAYLOAD);
  auto payload_event = std::dynamic_pointer_cast<PayloadEvent>(event);
  ASSERT_EQ(payload_event->GetPayload().size(), payload.size());
  for (size_t i = 0; i < payload.size(); ++i) {
    EXPECT_EQ(payload_event->GetPayload().at(i), payload.at(i));
  }
}

TEST_F(ConnectionManagers, SendMessages) {
  create_server_and_client();
  for (int i = 0; i < 10; ++i) {
    std::vector<uint8_t> payload_server_to_client{1, 2, 3, static_cast<uint8_t>(i)};
    std::vector<uint8_t> payload_client_to_server{5, 6, 7, static_cast<uint8_t>(i)};

    server_manager->SendDataToConnection(client_id, payload_server_to_client);
    client_manager->SendDataToConnection(server_id, payload_client_to_server);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    server_manager->HandleConnections();
    client_manager->HandleConnections();

    assert_payload_received(server_manager->PopAndGetOldestEvent(), payload_client_to_server);
    assert_payload_received(client_manager->PopAndGetOldestEvent(), payload_server_to_client);

    // all events should have been processed by now
    EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
    EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
  }
}

TEST_F(ConnectionManagers, TwoClients) {
  create_server_and_client();
  create_second_client();

  for (int i = 0; i < 10; ++i) {
    std::vector<uint8_t> payload_server_to_client1{1, 2, static_cast<uint8_t>(i)};
    std::vector<uint8_t> payload_server_to_client2{3, 4, static_cast<uint8_t>(i)};
    std::vector<uint8_t> payload_client1_to_server{5, 6, static_cast<uint8_t>(i)};
    std::vector<uint8_t> payload_client2_to_server{7, 8, static_cast<uint8_t>(i)};

    server_manager->SendDataToConnection(client_id, payload_server_to_client1);
    server_manager->SendDataToConnection(second_client_id, payload_server_to_client2);
    client_manager->SendDataToConnection(server_id, payload_client1_to_server);

    std::this_thread::sleep_for(std::chrono::microseconds(100));
    server_manager->HandleConnections();

    second_client_manager->SendDataToConnection(server_id, payload_client2_to_server);

    std::this_thread::sleep_for(std::chrono::microseconds(100));
    server_manager->HandleConnections();
    client_manager->HandleConnections();
    second_client_manager->HandleConnections();

    assert_payload_received(server_manager->PopAndGetOldestEvent(), payload_client1_to_server);
    assert_payload_received(server_manager->PopAndGetOldestEvent(), payload_client2_to_server);
    assert_payload_received(client_manager->PopAndGetOldestEvent(), payload_server_to_client1);
    assert_payload_received(second_client_manager->PopAndGetOldestEvent(), payload_server_to_client2);

    // all events should have been processed by now
    EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
    EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
    EXPECT_EQ(second_client_manager->PopAndGetOldestEvent(), nullptr);
  }
}

TEST_F(ConnectionManagers, LongQueue) {
  create_server_and_client();
  std::vector<std::vector<uint8_t>> payloads_server_to_client;
  std::vector<std::vector<uint8_t>> payloads_client_to_server;

  int count = 10;

  for (int i = 0; i < count; ++i) {
    payloads_server_to_client.emplace_back();
    payloads_server_to_client.back().push_back(1);
    payloads_server_to_client.back().push_back(i);
    payloads_server_to_client.back().push_back(2);

    payloads_client_to_server.emplace_back();
    payloads_client_to_server.back().push_back(3);
    payloads_client_to_server.back().push_back(i);
    payloads_client_to_server.back().push_back(4);
  }

  for (int i = 0; i < count; ++i) {
    server_manager->SendDataToConnection(client_id, payloads_server_to_client.at(i));
    client_manager->SendDataToConnection(server_id, payloads_client_to_server.at(i));
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    server_manager->HandleConnections();
    client_manager->HandleConnections();
  }

  for (int i = 0; i < count; ++i) {
    assert_payload_received(server_manager->PopAndGetOldestEvent(), payloads_client_to_server.at(i));
    assert_payload_received(client_manager->PopAndGetOldestEvent(), payloads_server_to_client.at(i));
  }

  // all events should have been processed by now
  EXPECT_EQ(server_manager->PopAndGetOldestEvent(), nullptr);
  EXPECT_EQ(client_manager->PopAndGetOldestEvent(), nullptr);
}

TEST_F(ConnectionManagers, ClientConnectionReset) {
  // Client sends message of type ConnectionResetMessage
  // set very high timeout, Connection Reset is triggered by message
  ProtocolDefinition::timeout_t timeout = std::chrono::milliseconds(7000);

  std::shared_ptr<ServerSideConnectionManager> server_manager = ServerSideConnectionManager::CreateServerSide(timeout);
  std::thread server_thread([&server_manager] {
    int counter = 10;
    while (counter > 0 && !server_manager->ConnectionCount()) {
      server_manager->HandleConnections();
      std::this_thread::sleep_for(std::chrono::microseconds(10));
      auto event = server_manager->PopAndGetOldestEvent();
      if (event) {
        if (event->GetType() == EventType::DISCONNECT) {
          ASSERT_FALSE(server_manager->ConnectionCount());
        }
      }
      counter--;
    }
  });

  auto client_manager = ClientSideConnectionManager::CreateClientSide(timeout);
  client_manager->HandleConnections();

  // client is successfully connected
  EXPECT_EQ(client_manager->PopAndGetOldestEvent()->GetType(), EventType::CONNECT);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // reset Connection
  auto reset_message = ConnectionResetMessage();
  client_manager->SendToConnection(ProtocolDefinition::server_partner_id, &reset_message);
  server_thread.join();
}

TEST_F(ConnectionManagers, ServerConnectionReset) {
  // Server sends message of type ConnectionResetMessage
  // set very high timeout, Connection Reset is triggered by message
  ProtocolDefinition::timeout_t timeout = std::chrono::milliseconds(7000);

  std::shared_ptr<ServerSideConnectionManager> server_manager = ServerSideConnectionManager::CreateServerSide(timeout);
  std::thread server_thread([&server_manager] {
    int counter = 10;
    while (!server_manager->ConnectionCount() || counter > 0) {
      server_manager->HandleConnections();
      std::this_thread::sleep_for(std::chrono::microseconds(10));
      counter--;
    }

    // Server is connected to client
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    auto event = server_manager->PopAndGetOldestEvent();
    if (event) {
      ASSERT_EQ(event->GetType(), EventType::CONNECT);
    }

    // reset Connection
    auto reset_message = ConnectionResetMessage();
    server_manager->SendToConnection(1, &reset_message);
  });
  auto client_manager = ClientSideConnectionManager::CreateClientSide(timeout);
  server_thread.join();
  int count = 10;
  while (count > 0) {
    client_manager->HandleConnections();
    auto event = client_manager->PopAndGetOldestEvent();
    if (event) {
      if (event->GetType() == EventType::DISCONNECT) {
        ASSERT_TRUE(true);
      }
    }
    count--;
  }
}
