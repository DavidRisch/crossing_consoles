#include "ClientSideConnectionManager.h"

#include "../message_layer/message/KeepAliveMessage.h"

using namespace communication;
using namespace connection_layer;

ClientSideConnectionManager::ClientSideConnectionManager(time_t timeout)
    : ConnectionManager(timeout) {
}

std::shared_ptr<ClientSideConnectionManager> ClientSideConnectionManager::CreateClientSide(time_t timeout) {
  auto byte_stream = byte_layer::SocketByteStream::CreateClientSide();
  auto message_input_stream = std::make_shared<message_layer::MessageInputStream>(byte_stream);
  auto message_output_stream = std::make_shared<message_layer::MessageOutputStream>(byte_stream);
  std::shared_ptr<Connection> connection =
      Connection::CreateClientSide(std::move(message_input_stream), std::move(message_output_stream));
  auto manager = std::shared_ptr<ClientSideConnectionManager>(new ClientSideConnectionManager(timeout));
  manager->AddConnection(connection);
  return manager;
}

void ClientSideConnectionManager::HandleConnections() {
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
