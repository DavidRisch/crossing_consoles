#include <gtest/gtest.h>

#include <thread>

#include "../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../src/communication/connection_layer/ClientSideConnectionManager.h"
#include "../src/communication/connection_layer/ServerSideConnectionManager.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

TEST(ConnectionManager, ConnectClient) {
  std::shared_ptr<ServerSideConnectionManager> server_manager = ServerSideConnectionManager::CreateServerSide();
  std::thread server_thread([&server_manager] {
    int counter = 10;
    while (counter > 0) {
      server_manager->HandleConnections();
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      counter--;
    }
  });

  auto client_manager = ClientSideConnectionManager::CreateClientSide();
  server_thread.join();
}

TEST(ConnectionManager, ServerTimeout) {
  // Server becomes unreachable
  std::shared_ptr<ServerSideConnectionManager> server_manager = ServerSideConnectionManager::CreateServerSide();
  std::thread server_thread([&server_manager] {
    int counter = 10;
    while (counter > 0) {
      server_manager->HandleConnections();
      std::this_thread::sleep_for(std::chrono::microseconds(10));
      std::list<Message> queue = server_manager->GetMessageQueue();
      counter--;
    }
  });

  // set low timeout
  auto client_manager = ClientSideConnectionManager::CreateClientSide(static_cast<time_t>(0.01));
  server_thread.join();
  ASSERT_THROW(client_manager->HandleConnections(), ConnectionManager::ConnectionTimeout);
}

TEST(ConnectionManager, ClientTimeout) {
  // Client becomes unreachable
  std::shared_ptr<ServerSideConnectionManager> server_manager =
      ServerSideConnectionManager::CreateServerSide(static_cast<time_t>(0.01));
  std::thread server_thread([&server_manager] {
    int counter = 10;
    while (counter > 0) {
      try {
        server_manager->HandleConnections();
      } catch (ConnectionManager::ConnectionTimeout &e) {
        // timeout should only occur if connection has been established
        ASSERT_TRUE(server_manager->HasConnections());
      }
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      counter--;
    }
  });
  auto client_manager = ClientSideConnectionManager::CreateClientSide();
  server_thread.join();
}
