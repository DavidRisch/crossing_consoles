#ifndef CROSSING_CONSOLES_CONNECTIONMANAGER_H
#define CROSSING_CONSOLES_CONNECTIONMANAGER_H

#include <list>
#include <unordered_map>

#include "../../byte_layer/byte_stream/SocketByteServer.h"
#include "../../message_layer/message/Message.h"
#include "../../message_layer/message/PayloadMessage.h"
#include "../event/Event.h"
#include "Connection.h"

namespace communication {
namespace connection_layer {

struct ConnectionParameters {
  std::shared_ptr<connection_layer::Connection> connection;
  // timestamp_last_received holds the timestamp of the last received message from its associated connection
  // used for timeout identification
  std::chrono::time_point<std::chrono::steady_clock> timestamp_last_received;
};

using partner_id_t = ProtocolDefinition::partner_id_t;

class ConnectionManager {
 public:
  explicit ConnectionManager(ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout);

  /**
   * \brief Send data to all clients.
   */
  void Broadcast(const std::vector<uint8_t>& data);

  /**
   * \brief Send data to specified client.
   */
  void SendToConnection(partner_id_t partner_id, std::vector<uint8_t> data);

  /**
   * \brief Needs to be implemented on Server and Client side separately.
   */
  virtual void HandleConnections() = 0;

  /**
   * \brief Add connection to `connection_map`.
   * \details all connections on `connection_map` are managed by `HandleConnections`.
   */
  void AddConnection(const std::shared_ptr<Connection>& connection);

  /**
   * \brief Reset connection after timeout.
   */
  void ResetConnection(partner_id_t partner_id, const Connection& connection);

  /**
   * \brief Return and pop the oldest `Event`.
   */
  std::shared_ptr<Event> PopAndGetOldestEvent();

  /**
   * \brief Thrown if timeout occurred.
   */
  class TimeoutException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "Connection timeout occurred.";
    }
  };

  /**
   * \brief Thrown if no partner with with the given id could be found.
   */
  class UnknownPartnerException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "No partner with that id found.";
    }
  };

 protected:
  /**
   * \brief handle received messages and timeouts.
   */
  void ReceiveMessages();

  /// connection_map of connection id/ address : ConnectionParameters<connection, timestamp_last_received>
  std::unordered_map<partner_id_t, ConnectionParameters> connection_map;

 private:
  virtual partner_id_t GetNextPartnerId() = 0;

  /// Unprocessed events ordered from oldest to newest.
  std::list<std::shared_ptr<Event>> event_queue;
  ProtocolDefinition::timeout_t timeout;  // timeout in milliseconds
};

}  // namespace connection_layer
}  // namespace communication
#endif  // CROSSING_CONSOLES_CONNECTIONMANAGER_H