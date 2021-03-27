#ifndef CROSSING_CONSOLES_BYTE_SERVER_H
#define CROSSING_CONSOLES_BYTE_SERVER_H

#include <cstdint>
#include <optional>
#include <stdexcept>

#include "SocketByteStream.h"

namespace communication {
namespace byte_layer {

/**
 * \brief Creates a socket to listen for clients and creates `SocketByteStream`s to communicate with them.
 */
class SocketByteServer {
 public:
  explicit SocketByteServer(
      const std::shared_ptr<byte_layer::IConnectionSimulatorProvider>& connection_simulator_provider =
          byte_layer::PerfectConnectionSimulatorProvider::instance,
      uint16_t port = socket_default_port, int max_connections = 5);

  /**
   * \brief Create `SocketByteStream` for a new client.
   * \details If no clients want to connect, do nothing. If clients are waiting for a connection, accept a single on of
   * them and create a new `SocketByteStream` to it.
   */
  std::shared_ptr<SocketByteStream> GetNewClient();

 private:
  std::shared_ptr<SocketHolder> socket_holder;

  std::shared_ptr<byte_layer::IConnectionSimulatorProvider> connection_simulator_provider;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_BYTE_SERVER_H
