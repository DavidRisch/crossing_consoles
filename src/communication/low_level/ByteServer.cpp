#include "ByteServer.h"

#include "socket_libs.h"

ByteServer::ByteServer(port_t port, int max_connections) {
  {
    file_descriptor_t server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    // Create socket
    if ((server_file_descriptor) < 0) {
      throw std::runtime_error("socket failed");
    }

    socket_ptr = std::make_shared<Socket>(server_file_descriptor);
  }

  // Configure socket
  int opt_val = 1;
  if (setsockopt(socket_ptr->file_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt_val, sizeof(opt_val))) {
    throw std::runtime_error("setsockopt failed");
  }

  struct sockaddr_in address {};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;

  address.sin_port = htons(port);

  // Attach to port
  if (bind(socket_ptr->file_descriptor, (struct sockaddr *)&address, sizeof(address)) < 0) {
    throw std::runtime_error("bind failed");
  }
  if (listen(socket_ptr->file_descriptor, max_connections) < 0) {
    throw std::runtime_error("listen failed");
  }

  // None blocking mode (needed for GetNewClient())
#ifdef _WIN32
  // Windows
  u_long blocking_io_mode = 1;  // != 0 -> none blocking mode
  if (ioctlsocket(socket->file_descriptor, FIONBIO, &blocking_io_mode) != 0) {
    throw std::runtime_error("ioctlsocket failed");
  }
#else
  // Linux
  if (fcntl(socket_ptr->file_descriptor, F_SETFL, fcntl(socket_ptr->file_descriptor, F_GETFL, 0) | O_NONBLOCK) != 0) {
    throw std::runtime_error("fcntl failed");
  }
#endif
}

std::optional<ByteStream> ByteServer::GetNewClient() {  // NOLINT(readability-make-member-function-const)
  struct sockaddr_in address {};
  int address_length = sizeof(address);

  file_descriptor_t socket_file_descriptor =
      accept(socket_ptr->file_descriptor, (struct sockaddr *)&address, (socklen_t *)&address_length);
  if (socket_file_descriptor < 0) {
    if (errno == EAGAIN) {
      // No client is trying to connect
      return std::optional<ByteStream>();
    }
    throw std::runtime_error("accept failed");
  }

  return std::make_optional<ByteStream>(socket_file_descriptor);
}
