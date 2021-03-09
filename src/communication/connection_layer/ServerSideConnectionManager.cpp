#include "ServerSideConnectionManager.h"

#include <iostream>

#include "ConnectionManager.h"

using namespace communication;
using namespace connection_layer;

ServerSideConnectionManager::ServerSideConnectionManager(ProtocolDefinition::ms_count_t timeout)
    : ConnectionManager(timeout) {
}

void ServerSideConnectionManager::HandleConnections() {
  // Check for and establish new connections
  auto new_client = byte_server->GetNewClient();
  if (new_client != nullptr) {
    auto message_input_stream = std::make_shared<message_layer::MessageInputStream>(new_client);
    auto message_output_stream = std::make_shared<message_layer::MessageOutputStream>(new_client);
    std::shared_ptr<Connection> connection =
        Connection::CreateServerSide(std::move(message_input_stream), std::move(message_output_stream));
    AddConnection(connection);
  }
  ReceiveMessages();
}

std::shared_ptr<ServerSideConnectionManager> ServerSideConnectionManager::CreateServerSide(
    ProtocolDefinition::ms_count_t timeout) {
  std::shared_ptr<ServerSideConnectionManager> manager =
      std::shared_ptr<ServerSideConnectionManager>(new ServerSideConnectionManager(timeout));
  manager->byte_server = std::make_shared<byte_layer::SocketByteServer>();
  return manager;
}
bool ServerSideConnectionManager::HasConnections() {
  return !connection_map.empty();
}
