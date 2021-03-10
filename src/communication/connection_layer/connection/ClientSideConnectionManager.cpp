#include "ClientSideConnectionManager.h"

#include <iostream>

using namespace communication;
using namespace connection_layer;

ClientSideConnectionManager::ClientSideConnectionManager(ProtocolDefinition::timeout_t timeout)
    : ConnectionManager(timeout) {
}

std::shared_ptr<ClientSideConnectionManager> ClientSideConnectionManager::CreateClientSide(
    ProtocolDefinition::timeout_t timeout) {
  auto byte_stream = byte_layer::SocketByteStream::CreateClientSide();
  auto message_input_stream = std::make_shared<message_layer::MessageInputStream>(byte_stream);
  auto message_output_stream = std::make_shared<message_layer::MessageOutputStream>(byte_stream);
  std::shared_ptr<Connection> connection =
      Connection::CreateClientSide(std::move(message_input_stream), std::move(message_output_stream), timeout);
  auto manager = std::shared_ptr<ClientSideConnectionManager>(new ClientSideConnectionManager(timeout));
  manager->AddConnection(connection);
  return manager;
}

void ClientSideConnectionManager::HandleConnections() {
  ReceiveMessages();
}

address_t ClientSideConnectionManager::GetNextPartnerId() {
  return ProtocolDefinition::server_partner_id;  // constant because only a single server can exist.
}
