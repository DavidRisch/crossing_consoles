#ifndef CROSSING_CONSOLES_BYTE_SERVER_H
#define CROSSING_CONSOLES_BYTE_SERVER_H

#include <cstdint>
#include <optional>
#include <stdexcept>

#include "ByteStream.h"

/**
 * Creates a socket to listen for clients and creates ByteStreams to communicate with them.
 */
class ByteServer {
 public:
  explicit ByteServer(uint16_t port = socket_default_port, int max_connections = 5);

  std::optional<ByteStream> GetNewClient();

 private:
  std::shared_ptr<Socket> socket_ptr;
};

#endif  // CROSSING_CONSOLES_BYTE_SERVER_H
