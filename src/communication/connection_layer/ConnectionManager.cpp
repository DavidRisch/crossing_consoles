#include "ConnectionManager.h"

#include <utility>

#include "../message_layer/message/KeepAliveMessage.h"

using namespace communication;
using namespace connection_layer;

ConnectionManager::ConnectionManager(time_t timeout)
    : timeout(timeout) {
  connection_map = {};
}

void ConnectionManager::Broadcast(const std::vector<uint8_t>& payload) {
  std::map<message_layer::address_t, connection_layer::Connection>::iterator connection_it;
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
  connection_map.insert({id, ConnectionParameters{connection, std::time(nullptr)}});
}

std::list<message_layer::Message> ConnectionManager::GetMessageQueue() {
  std::list<message_layer::Message> tmp(message_queue);
  message_queue.clear();
  return tmp;
}
