#ifndef CROSSING_CONSOLES_CLIENTSIDECONNECTIONMANAGER_H
#define CROSSING_CONSOLES_CLIENTSIDECONNECTIONMANAGER_H

#include "ConnectionManager.h"

namespace communication {
namespace connection_layer {

class ClientSideConnectionManager : public ConnectionManager {
 public:
  static std::shared_ptr<ClientSideConnectionManager> CreateClientSide(
      ProtocolDefinition::timeout_t timeout = ProtocolDefinition::timeout);
  /**
   * \brief Check for and process new messages and timeouts.
   * \details On client side only the server connection is managed.
   */
  void HandleConnections() override;

 private:
  ClientSideConnectionManager(ProtocolDefinition::timeout_t timeout);

  partner_id_t GetNextPartnerId() override;
};
}  // namespace connection_layer
}  // namespace communication
#endif  // CROSSING_CONSOLES_CLIENTSIDECONNECTIONMANAGER_H
