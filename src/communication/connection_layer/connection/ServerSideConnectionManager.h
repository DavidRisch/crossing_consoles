#ifndef CROSSING_CONSOLES_SERVERSIDECONNECTIONMANAGER_H
#define CROSSING_CONSOLES_SERVERSIDECONNECTIONMANAGER_H

#include "ConnectionManager.h"

namespace communication {
namespace connection_layer {

/**
 * \brief ConnectionManager on the Server Side
 */

class ServerSideConnectionManager : public ConnectionManager {
 public:
  /**
   * \brief Check for and process new connections, messages and timeouts.
   */
  void HandleConnections() override;
  /**
   * \brief Create Connection Manager for usage on server side.
   */
  static std::shared_ptr<ServerSideConnectionManager> CreateServerSide(
      ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout,
      const std::shared_ptr<byte_layer::IConnectionSimulatorProvider>& connection_simulator_provider =
          byte_layer::PerfectConnectionSimulatorProvider::instance);

  /**
   * \brief Returns the number of connected clients.
   */
  int ConnectionCount();

 private:
  ServerSideConnectionManager(
      ProtocolDefinition::timeout_t timeout,
      const std::shared_ptr<byte_layer::IConnectionSimulatorProvider>& connection_simulator_provider);

  partner_id_t GetNextPartnerId() override;

  partner_id_t next_partner_id = ProtocolDefinition::server_partner_id + 1;

  std::shared_ptr<byte_layer::SocketByteServer> byte_server;
};
}  // namespace connection_layer
}  // namespace communication
#endif  // CROSSING_CONSOLES_SERVERSIDECONNECTIONMANAGER_H
