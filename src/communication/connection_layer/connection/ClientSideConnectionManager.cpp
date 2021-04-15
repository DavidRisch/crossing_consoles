#include "ClientSideConnectionManager.h"

#include <iostream>
#include <utility>

#include "../../debug.h"

using namespace communication;
using namespace connection_layer;

ClientSideConnectionManager::ClientSideConnectionManager(ProtocolDefinition::timeout_t timeout)
    : ConnectionManager(timeout) {
}

std::shared_ptr<ClientSideConnectionManager> ClientSideConnectionManager::CreateClientSide(
    ProtocolDefinition::timeout_t timeout,
    std::shared_ptr<byte_layer::IConnectionSimulatorProvider> connection_simulator_provider) {
  auto byte_stream = byte_layer::SocketByteStream::CreateClientSide(std::move(connection_simulator_provider));
  byte_stream->SetParamCatchSendFailed(false);
  auto message_input_stream = std::make_shared<message_layer::MessageInputStream>(byte_stream);
  auto message_output_stream = std::make_shared<message_layer::MessageOutputStream>(byte_stream);
  std::shared_ptr<Connection> connection =
      Connection::CreateClientSide(std::move(message_input_stream), std::move(message_output_stream), timeout);

  connection->BlockingEstablish();
  DEBUG_CONNECTION_LOG(connection.get(), "Establish done (ClientSideConnectionManager)")

  auto manager = std::shared_ptr<ClientSideConnectionManager>(new ClientSideConnectionManager(timeout));
  manager->AddConnection(connection);
  return manager;
}

void ClientSideConnectionManager::HandleConnections(std::chrono::steady_clock::time_point now) {
  for (const auto& pair : connection_map) {
    pair.second.connection->Handle(now);
  }
  ReceiveMessages(now);
}

partner_id_t ClientSideConnectionManager::GetNextPartnerId() {
  return ProtocolDefinition::server_partner_id;  // constant because only a single server can exist.
}

void ClientSideConnectionManager::SendMessageToServer(const std::shared_ptr<message_layer::Message>& msg) {
  SendMessageToConnection(ProtocolDefinition::server_partner_id, msg);
}

void ClientSideConnectionManager::SendDataToServer(std::vector<uint8_t> data) {
  SendDataToConnection(ProtocolDefinition::server_partner_id, std::move(data));
}

void ClientSideConnectionManager::CloseConnectionWithServer() {
  CloseConnection(ProtocolDefinition::server_partner_id);
}
