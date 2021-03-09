#include "ConnectionManager.h"

#include <chrono>
#include <utility>

#include "../message_layer/message/KeepAliveMessage.h"

using namespace communication;
using namespace connection_layer;

ConnectionManager::ConnectionManager(ProtocolDefinition::ms_count_t timeout)
    : timeout(timeout) {
  connection_map = {};
}

void ConnectionManager::Broadcast(const std::vector<uint8_t>& payload) {
  for (auto& connection_entry : connection_map) {
    SendToConnection(connection_entry.first, payload);
  }
}

void ConnectionManager::SendToConnection(message_layer::address_t address, std::vector<uint8_t> data) {
  // TODO catch exception if address is not in keys
  auto connection = connection_map.at(address).connection;
  message_layer::PayloadMessage msg = message_layer::PayloadMessage(address, std::move(data));
  connection->SendMessage(&msg);
}

void ConnectionManager::ResetConnection(message_layer::address_t address, const Connection& client_connection) {
  // TODO Reset connection
}

void ConnectionManager::AddConnection(const std::shared_ptr<Connection>& connection) {
  message_layer::address_t id = 0;
  if (!connection_map.empty()) {
    id = connection_map.rbegin()->first;
  }
  connection_map.insert({id, ConnectionParameters{connection, std::chrono::steady_clock::now()}});
}

std::list<message_layer::Message> ConnectionManager::GetAndClearMessageQueue() {
  std::list<message_layer::Message> tmp(message_queue);
  message_queue.clear();
  return tmp;
}

void ConnectionManager::ReceiveMessages() {
  // Handle received messages and timeouts
  for (auto& connection_entry : connection_map) {
    auto connection = connection_entry.second.connection;
    auto address = connection_entry.first;

    auto msg = message_layer::KeepAliveMessage(address);
    connection->SendMessage(&msg);

    std::shared_ptr<message_layer::Message> received_msg;
    do {
      received_msg = connection->ReceiveMessage();
      if (received_msg != nullptr) {
        connection_entry.second.timestamp_last_received = std::chrono::steady_clock::now();
        if (received_msg->GetMessageType() == message_layer::MessageType::PAYLOAD) {
          message_queue.push_back(*received_msg);
        }
      }
    } while (received_msg != nullptr);

    if (std::chrono::steady_clock::now() - connection_entry.second.timestamp_last_received >= timeout) {
      // TODO handle timeout, reset connection
      throw ConnectionManager::ConnectionTimeout();
    }
  }
}
