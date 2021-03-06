#ifndef CROSSING_CONSOLES_CONNECTIONMANAGER_H
#define CROSSING_CONSOLES_CONNECTIONMANAGER_H

#include <list>
#include <unordered_map>

#include "../../byte_layer/byte_stream/SocketByteServer.h"
#include "../../byte_layer/connection_simulator/ConnectionSimulatorPerfect.h"
#include "../../byte_layer/connection_simulator/IConnectionSimulatorProvider.h"
#include "../../byte_layer/connection_simulator/PerfectConnectionSimulatorProvider.h"
#include "../../message_layer/message/Message.h"
#include "../../message_layer/message/PayloadMessage.h"
#include "../event/Event.h"
#include "Connection.h"

namespace communication {
namespace connection_layer {

/**
 * \brief Holds a pointer to the `Connection` and the time when the last `Message` was received on it.
 */
struct ConnectionParameters {
  std::shared_ptr<connection_layer::Connection> connection;
  // timestamp_last_received holds the timestamp of the last received message from its associated connection
  // used for timeout identification
  std::chrono::time_point<std::chrono::steady_clock> timestamp_last_received;
};

using partner_id_t = ProtocolDefinition::partner_id_t;

/**
 * \brief Base class for handling `Connection`s on the Server or the Client.
 */
class ConnectionManager {
 public:
  explicit ConnectionManager(
      ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout,
      const std::shared_ptr<byte_layer::IConnectionSimulatorProvider>& connection_simulator_provider =
          byte_layer::PerfectConnectionSimulatorProvider::instance);

  virtual ~ConnectionManager() = default;

  /**
   * \brief Send data to all clients.
   */
  void Broadcast(const std::vector<uint8_t>& data);

  /**
   * \brief Send data to specified client.
   */
  void SendDataToConnection(partner_id_t partner_id, std::vector<uint8_t> data);

  /**
   * \brief Send message to specified client.
   */
  void SendMessageToConnection(partner_id_t partner_id, const std::shared_ptr<message_layer::Message>& msg);

  /**
   * \brief Needs to be implemented on Server and Client side separately.
   */
  virtual void HandleConnections(std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now()) = 0;

  /**
   * \brief Add connection to `connection_map`.
   * \details all connections on `connection_map` are managed by `HandleConnections`.
   */
  void AddConnection(const std::shared_ptr<Connection>& connection);

  /**
   * \brief Notify connection partner that connection will be closed,
   * \details Send ResetConnection message, wait for acknowledge message before actually closing the connection.
   */
  void CloseConnection(partner_id_t partner_id);

  /**
   * \brief Test if a new `Event` can be retrieved by `PopAndGetOldestEvent()`.
   */
  bool HasEvent();

  /**
   * \brief Return and pop the oldest `Event`.
   */
  std::shared_ptr<Event> PopAndGetOldestEvent();

  /**
   * \brief False if 'connection_map` is empty.
   */
  bool HasConnections();

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

  /**
   * Return connection statistics of partner connection
   */
  const ConnectionStatistics& GetStatisticsFromPartnerConnection(partner_id_t partner_id) const;

 protected:
  /**
   * \brief handle received messages and timeouts.
   */
  void ReceiveMessages(std::chrono::steady_clock::time_point now);

  /// connection_map of connection id/ address : ConnectionParameters<connection, timestamp_last_received>
  std::unordered_map<partner_id_t, ConnectionParameters> connection_map;

 private:
  /**
   * \brief Remove connection from `connection_map` .
   * \details sets `DisconnectEvent` in `event_queue`.
   */
  void RemoveConnection(partner_id_t partner_id);

  virtual partner_id_t GetNextPartnerId() = 0;

  /// Unprocessed events ordered from oldest to newest.
  std::list<std::shared_ptr<Event>> event_queue;
  ProtocolDefinition::timeout_t timeout;  // timeout in milliseconds

  /// Set the interval of sent KeepAliveMessages in dependency of `timeout`
  ProtocolDefinition::timeout_t keep_alive_interval{};

  ProtocolDefinition::timeout_t resend_interval{};

 protected:
  std::shared_ptr<byte_layer::IConnectionSimulatorProvider> connection_simulator_provider;
};

}  // namespace connection_layer
}  // namespace communication
#endif  // CROSSING_CONSOLES_CONNECTIONMANAGER_H
