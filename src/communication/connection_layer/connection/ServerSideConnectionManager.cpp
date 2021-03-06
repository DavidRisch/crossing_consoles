#include "ServerSideConnectionManager.h"

#include <utility>

#include "../../debug.h"
#include "ConnectionManager.h"

using namespace communication;
using namespace connection_layer;

ServerSideConnectionManager::ServerSideConnectionManager(
    ProtocolDefinition::timeout_t timeout,
    const std::shared_ptr<byte_layer::IConnectionSimulatorProvider>& connection_simulator_provider)
    : ConnectionManager(timeout, connection_simulator_provider) {
}

void ServerSideConnectionManager::HandleConnections(std::chrono::steady_clock::time_point now) {
  // Check for and establish new connections
  auto new_client = byte_server->GetNewClient();
  if (new_client != nullptr) {
    new_client->SetParamCatchSendFailed(false);
    auto message_input_stream = std::make_shared<message_layer::MessageInputStream>(new_client);
    auto message_output_stream = std::make_shared<message_layer::MessageOutputStream>(new_client);

    std::shared_ptr<Connection> connection =
        Connection::CreateServerSide(std::move(message_input_stream), std::move(message_output_stream));

    connection->BlockingEstablish();
    DEBUG_CONNECTION_LOG(connection.get(), "Establish done (ServerSideConnectionManager)\n")

    AddConnection(connection);
  }

  FastHandleConnections(now);
}

void ServerSideConnectionManager::FastHandleConnections(std::chrono::steady_clock::time_point now) {
  ReceiveMessages(now);

  for (auto& entry : connection_map) {
    entry.second.connection->Handle(now);
  }
}

std::shared_ptr<ServerSideConnectionManager> ServerSideConnectionManager::CreateServerSide(
    ProtocolDefinition::timeout_t timeout,
    const std::shared_ptr<byte_layer::IConnectionSimulatorProvider>& connection_simulator_provider) {
  auto manager = std::shared_ptr<ServerSideConnectionManager>(
      new ServerSideConnectionManager(timeout, connection_simulator_provider));
  manager->byte_server = std::make_shared<byte_layer::SocketByteServer>(connection_simulator_provider);
  return manager;
}

int ServerSideConnectionManager::ConnectionCount() {
  return connection_map.size();
}

partner_id_t ServerSideConnectionManager::GetNextPartnerId() {
  return next_partner_id++;
}
