#include "ServerSideConnectionManager.h"

#include "../message_layer/message/KeepAliveMessage.h"
#include "ConnectionManager.h"

using namespace communication;
using namespace connection_layer;

ServerSideConnectionManager::ServerSideConnectionManager(time_t timeout)
    : ConnectionManager(timeout) {
}

void ServerSideConnectionManager::HandleConnections() {
  // Check for new connections and establish connection
  auto new_client = byte_server->GetNewClient();
  if (new_client != nullptr) {
    auto message_input_stream = std::make_shared<message_layer::MessageInputStream>(new_client);
    auto message_output_stream = std::make_shared<message_layer::MessageOutputStream>(new_client);
    std::shared_ptr<Connection> connection =
        Connection::CreateServerSide(std::move(message_input_stream), std::move(message_output_stream));
    AddConnection(connection);
  }

  // Handle received messages and timeouts
  for (auto& connection_entry : connection_map) {
    auto connection = connection_entry.second.connection;
    auto address = connection_entry.first;

    auto msg = message_layer::KeepAliveMessage(address);
    connection->SendMessage(&msg);

    std::shared_ptr<message_layer::Message> received_msg = connection->ReceiveMessage();
    while (received_msg) {
      if (received_msg != nullptr) {
        connection_entry.second.timestamp_last_received = std::time(nullptr);
        if (received_msg->GetMessageType() == message_layer::MessageType::PAYLOAD) {
          message_queue.push_back(*received_msg);
        }
      }
      received_msg = connection->ReceiveMessage();
    }
    if (std::time(nullptr) - connection_entry.second.timestamp_last_received >= timeout) {
      // TODO handle timeout, reset connection
      throw ConnectionManager::ConnectionTimeout();
    }
  }
}

std::shared_ptr<ServerSideConnectionManager> ServerSideConnectionManager::CreateServerSide(time_t timeout) {
  std::shared_ptr<ServerSideConnectionManager> manager =
      std::shared_ptr<ServerSideConnectionManager>(new ServerSideConnectionManager(timeout));
  manager->byte_server = std::make_shared<byte_layer::SocketByteServer>();
  return manager;
}
bool ServerSideConnectionManager::HasConnections() {
  return !connection_map.empty();
}
