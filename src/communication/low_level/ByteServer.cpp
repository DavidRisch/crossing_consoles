#include "ByteServer.h"

namespace socket {
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

// required for INADDR_ANY macro to work with this namespace setup
#define in_addr_t socket::in_addr_t
}  // namespace socket

#include <fcntl.h>

#include <cstring>

ByteServer::ByteServer(port_t port, int max_connections) {
  {
    file_descriptor_t server_file_descriptor = socket::socket(AF_INET, socket::SOCK_STREAM, 0);

    // Create socket
    if ((server_file_descriptor) < 0) {
      throw std::runtime_error("socket::socket failed");
    }

    socket = std::make_shared<Socket>(server_file_descriptor);
  }

  // Configure socket
  int opt_val = 1;
  if (socket::setsockopt(socket->file_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt_val, sizeof(opt_val))) {
    throw std::runtime_error("socket::setsockopt failed");
  }

  struct socket::sockaddr_in address {};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;

  address.sin_port = socket::htons(port);

  // Attach to port
  if (socket::bind(socket->file_descriptor, (struct socket::sockaddr *)&address, sizeof(address)) < 0) {
    throw std::runtime_error("socket::bind failed");
  }
  if (socket::listen(socket->file_descriptor, max_connections) < 0) {
    throw std::runtime_error("socket::listen failed");
  }

  // None blocking mode (needed for GetNewClient())
  if (fcntl(socket->file_descriptor, F_SETFL, fcntl(socket->file_descriptor, F_GETFL, 0) | O_NONBLOCK)) {
    throw std::runtime_error("fcntl failed");
  }
}

std::optional<ByteStream> ByteServer::GetNewClient() {  // NOLINT(readability-make-member-function-const)
  struct socket::sockaddr_in address {};
  int address_length = sizeof(address);

  file_descriptor_t socket_file_descriptor = socket::accept(
      socket->file_descriptor, (struct socket::sockaddr *)&address, (socket::socklen_t *)&address_length);
  if (socket_file_descriptor < 0) {
    if (errno == EAGAIN) {
      // No client is trying to connect
      return std::optional<ByteStream>();
    }
    throw std::runtime_error("socket::accept failed");
  }

  return std::make_optional<ByteStream>(socket_file_descriptor);
}
