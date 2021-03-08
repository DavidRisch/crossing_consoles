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
  static std::shared_ptr<ServerSideConnectionManager> CreateServerSide(time_t timeout = ProtocolDefinition::timeout);

  /**
   * \brief Returns true if server has connected clients.
   */
  bool HasConnections();

 private:
  ServerSideConnectionManager(time_t timeout);
  std::shared_ptr<byte_layer::SocketByteServer> byte_server;
};
}  // namespace connection_layer
}  // namespace communication
#endif  // CROSSING_CONSOLES_SERVERSIDECONNECTIONMANAGER_H
