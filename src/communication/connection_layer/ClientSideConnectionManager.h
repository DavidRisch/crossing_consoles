#ifndef CROSSING_CONSOLES_CLIENTSIDECONNECTIONMANAGER_H
#define CROSSING_CONSOLES_CLIENTSIDECONNECTIONMANAGER_H

#include "ConnectionManager.h"

namespace communication {
namespace connection_layer {

class ClientSideConnectionManager : public ConnectionManager {
 public:
  static std::shared_ptr<ClientSideConnectionManager> CreateClientSide(
      ProtocolDefinition::ms_count_t timeout = ProtocolDefinition::timeout);
  /**
   * \brief Check for and process new messages and timeouts.
   * \details On client side only the server connection is managed.
   */
  void HandleConnections() override;

 private:
  ClientSideConnectionManager(ProtocolDefinition::ms_count_t timeout);
};
}  // namespace connection_layer
}  // namespace communication
#endif  // CROSSING_CONSOLES_CLIENTSIDECONNECTIONMANAGER_H
