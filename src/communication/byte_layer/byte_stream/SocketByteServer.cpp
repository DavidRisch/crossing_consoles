#include "SocketByteServer.h"

#include "socket_libs.h"

using namespace communication;
using namespace communication::byte_layer;

SocketByteServer::SocketByteServer(port_t port, int max_connections) {
  {
#ifdef _WIN32
    WORD w_version_requested;
    WSADATA wsa_data;
    int error;

    w_version_requested = MAKEWORD(2, 2);
    error = WSAStartup(w_version_requested, &wsa_data);
    if (error != 0) {
      throw std::runtime_error("WSAStartup failed with error");
    }

    if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2) {
      WSACleanup();
      throw std::runtime_error("no usable Winsock.dll");
    }
#endif
    file_descriptor_t server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    // Create socket
    if ((server_file_descriptor) < 0) {
      throw std::runtime_error("socket failed");
    }

    socket_holder = std::make_shared<SocketHolder>(server_file_descriptor);
  }

  // Configure socket
  int opt_val = 1;
#ifdef _WIN32
  // Windows
  if (setsockopt(socket_holder->file_descriptor, SOL_SOCKET, SO_REUSEADDR, (char *)&opt_val, sizeof(opt_val))) {
    throw std::runtime_error("setsockopt failed");
  }
#else
  // Linux
  if (setsockopt(socket_holder->file_descriptor, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val))) {
    throw std::runtime_error("setsockopt failed");
  }
#endif

  struct sockaddr_in address {};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;

  address.sin_port = htons(port);

  // Attach to port
  if (bind(socket_holder->file_descriptor, (struct sockaddr *)&address, sizeof(address)) < 0) {
    throw std::runtime_error("bind failed");
  }
  if (listen(socket_holder->file_descriptor, max_connections) < 0) {
    throw std::runtime_error("listen failed");
  }

  // None blocking mode (needed for GetNewClient())
#ifdef _WIN32
  // Windows
  u_long blocking_io_mode = 1;  // != 0 -> none blocking mode
  if (ioctlsocket(socket_holder->file_descriptor, FIONBIO, &blocking_io_mode) != 0) {
    throw std::runtime_error("ioctlsocket failed");
  }
#else
  // Linux
  if (fcntl(socket_holder->file_descriptor, F_SETFL, fcntl(socket_holder->file_descriptor, F_GETFL, 0) | O_NONBLOCK) !=
      0) {
    throw std::runtime_error("fcntl failed");
  }
#endif
}

std::shared_ptr<SocketByteStream> SocketByteServer::GetNewClient() {  // NOLINT(readability-make-member-function-const)
  struct sockaddr_in address {};
  int address_length = sizeof(address);

#ifdef _WIN32
  // Windows
  file_descriptor_t socket_file_descriptor;
  socket_file_descriptor = accept(socket_holder->file_descriptor, (struct sockaddr *)&address, &address_length);

  int error = WSAGetLastError();

#else
  // Linux
  file_descriptor_t socket_file_descriptor =
      accept(socket_holder->file_descriptor, (struct sockaddr *)&address, (socklen_t *)&address_length);
#endif

  if (socket_file_descriptor < 0) {
#ifdef _WIN32
    if (error == WSAEWOULDBLOCK) {
#else
    if (errno == EAGAIN) {
#endif
      // No client is trying to connect
      return std::shared_ptr<SocketByteStream>();
    }
    throw std::runtime_error("accept failed");
  }

  auto socket_byte_stream = std::make_shared<SocketByteStream>(socket_file_descriptor);

  socket_byte_stream->ConfigureSocket();

  return socket_byte_stream;
}
