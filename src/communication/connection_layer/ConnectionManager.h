#ifndef CROSSING_CONSOLES_CONNECTIONMANAGER_H
#define CROSSING_CONSOLES_CONNECTIONMANAGER_H

#include <list>
#include <map>

#include "../byte_layer/byte_stream/SocketByteServer.h"
#include "../message_layer/message/Message.h"
#include "../message_layer/message/PayloadMessage.h"
#include "Connection.h"

namespace communication {
namespace connection_layer {

struct ConnectionParameters {
  std::shared_ptr<connection_layer::Connection> connection;
  // timestamp_last_received holds the timestamp of the last received message from its associated connection
  // used for timeout identification
  time_t timestamp_last_received;
};

class ConnectionManager {
 public:
  explicit ConnectionManager(time_t timeout = 0);
  /**
   * \brief Send data to all clients
   */
  void Broadcast(const std::vector<uint8_t>& data);
  /**
   * \brief Send data to specified client
   */
  void SendToConnection(message_layer::address_t address, std::vector<uint8_t> data);
  /**
   * \brief Needs to be implemented on Server and Client side separately
   */
  virtual void HandleConnections() = 0;
  /**
   * \brief Add connection to `connection_map`
   * \details all connections on `connection_map` are managed by `HandleConnections`
   */
  void AddConnection(const std::shared_ptr<Connection>& connection);

  /**
   * \brief Reset connection after timeout
   */
  void ResetConnection(message_layer::address_t address, const Connection& connection);

  /**
   * \brief Return all queued messages and empty queue
   */
  std::list<message_layer::Message> GetMessageQueue();

  class ConnectionTimeout : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "Connection timeout occurred.";
    }
  };

 protected:
  std::list<message_layer::Message> message_queue;
  time_t timeout;

  // connection_map of connection id/ address : ConnectionParameters<connection, timestamp_last_received>
  std::map<message_layer::address_t, ConnectionParameters> connection_map;
};

}  // namespace connection_layer
}  // namespace communication
#endif  // CROSSING_CONSOLES_CONNECTIONMANAGER_H
