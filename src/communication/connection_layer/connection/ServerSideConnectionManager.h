#ifndef CROSSING_CONSOLES_SERVERSIDECONNECTIONMANAGER_H
#define CROSSING_CONSOLES_SERVERSIDECONNECTIONMANAGER_H

#include "ConnectionManager.h"

namespace communication {
namespace connection_layer {

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
      ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout);

  /**
   * \brief Returns the number of connected clients.
   */
  int ConnectionCount();

 private:
  ServerSideConnectionManager(ProtocolDefinition::timeout_t timeout);

  address_t GetNextPartnerId() override;

  address_t next_partner_id = ProtocolDefinition::server_partner_id + 1;

  std::shared_ptr<byte_layer::SocketByteServer> byte_server;
};
}  // namespace connection_layer
}  // namespace communication
#endif  // CROSSING_CONSOLES_SERVERSIDECONNECTIONMANAGER_H
