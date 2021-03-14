#include "ConnectionManager.h"

#include <cassert>
#include <chrono>
#include <utility>

#include "../../message_layer/message/ConnectionResetMessage.h"
#include "../../message_layer/message/KeepAliveMessage.h"
#include "../event/ConnectEvent.h"
#include "../event/DisconnectEvent.h"
#include "../event/PayloadEvent.h"

using namespace communication;
using namespace connection_layer;

ConnectionManager::ConnectionManager(ProtocolDefinition::timeout_t timeout)
    : timeout(timeout) {
  connection_map = {};
}

void ConnectionManager::Broadcast(const std::vector<uint8_t>& payload) {
  for (auto& connection_entry : connection_map) {
    SendDataToConnection(connection_entry.first, payload);
  }
}

void ConnectionManager::SendDataToConnection(ProtocolDefinition::partner_id_t partner_id, std::vector<uint8_t> data) {
  auto msg = std::make_shared<message_layer::PayloadMessage>(std::move(data));
  SendMessageToConnection(partner_id, msg);
}

void ConnectionManager::RemoveConnection(ProtocolDefinition::partner_id_t partner_id) {
  auto connection_it = connection_map.find(partner_id);
  if (connection_it == connection_map.end()) {
    throw UnknownPartnerException();
  }
  event_queue.push_back(std::make_shared<DisconnectEvent>(partner_id));
  connection_it->second.connection->PrintStatistics();
  connection_map.erase(partner_id);
}

void ConnectionManager::AddConnection(const std::shared_ptr<Connection>& connection) {
  auto new_partner_id = GetNextPartnerId();
  connection_map.insert({new_partner_id, ConnectionParameters{connection, std::chrono::steady_clock::now()}});
  event_queue.push_back(std::make_shared<ConnectEvent>(new_partner_id));
}

std::shared_ptr<Event> ConnectionManager::PopAndGetOldestEvent() {
  if (event_queue.empty()) {
    return std::shared_ptr<Event>();
  }
  auto event = event_queue.front();
  event_queue.pop_front();
  return event;
}

void ConnectionManager::ReceiveMessages() {
  // Handle received messages and timeouts
  std::list<partner_id_t> connection_reset_list = {};
  std::list<partner_id_t> connection_close_list = {};

  for (auto& connection_entry : connection_map) {
    auto connection = connection_entry.second.connection;
    auto partner_id = connection_entry.first;

    /*
     TODO: send keep alive less frequently
     TODO: does not belong in this method
    auto msg = std::make_shared<message_layer::KeepAliveMessage>(partner_id);
    TODO: connection->SendMessage(msg);
     */

    std::shared_ptr<message_layer::Message> received_msg;
    do {
      received_msg = connection->ReceiveMessage();
      if (received_msg != nullptr) {
        connection_entry.second.timestamp_last_received = std::chrono::steady_clock::now();

        switch (received_msg->GetMessageType()) {
          case message_layer::MessageType::PAYLOAD: {
            const auto payload = std::dynamic_pointer_cast<message_layer::PayloadMessage>(received_msg)->GetPayload();
            event_queue.push_back(std::make_shared<PayloadEvent>(partner_id, payload));
            break;
          }
          case message_layer::MessageType::CONNECTION_RESET: {
            connection_reset_list.push_back(partner_id);
            break;
          }
          default: {
            continue;
          }
        }
      }
    } while (received_msg != nullptr);

    if (std::chrono::steady_clock::now() - connection_entry.second.timestamp_last_received >= timeout) {
      connection_close_list.push_back(partner_id);
    }
  }
  for (auto& partner_id : connection_close_list) {
    CloseConnection(partner_id);
  }

  for (auto& partner_id : connection_reset_list) {
    // only called if Connection Reset Message was received, connection can be removed immediately
    RemoveConnection(partner_id);
  }
}

void ConnectionManager::SendMessageToConnection(partner_id_t partner_id, std::shared_ptr<message_layer::Message> msg) {
  auto connection_it = connection_map.find(partner_id);
  if (connection_it == connection_map.end()) {
    throw UnknownPartnerException();
  }
  assert(connection_it->first == partner_id);
  auto connection = connection_it->second.connection;

  connection->SendMessage(msg);
}

void ConnectionManager::CloseConnection(partner_id_t partner_id) {
  // notify connection partner
  auto reset_msg = std::make_shared<message_layer::ConnectionResetMessage>();
  SendMessageToConnection(partner_id, reset_msg);
  // TODO wait for acknowledge message from partner before calling Reset Connection
  RemoveConnection(partner_id);
}
