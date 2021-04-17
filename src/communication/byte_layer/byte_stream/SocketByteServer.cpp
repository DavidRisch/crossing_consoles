#include "SocketByteServer.h"

#include <sys/stat.h>

#include "../../ProtocolDefinition.h"
#include "socket_libs.h"

using namespace communication;
using namespace communication::byte_layer;

SocketByteServer::SocketByteServer(
    const std::shared_ptr<byte_layer::IConnectionSimulatorProvider> &connection_simulator_provider, port_t port,
    int max_connections)
    : connection_simulator_provider(connection_simulator_provider) {
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
    file_descriptor_t server_file_descriptor = socket(SOCKET_PROTOCOL_FAMILY, SOCK_STREAM, 0);

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

#ifdef USE_UNIX_SOCKET
  std::string socket_path = ProtocolDefinition::GetUnixSocketPath(port);
  if (remove(socket_path.c_str()) != 0) {
    // ignore 'No such file or directory'
    if (errno != ENOENT) {
      throw std::runtime_error("remove failed " + std::to_string(errno));
    }
  }

  // All users are allowed to connect to this server
  umask(0);

  struct sockaddr_un address {};
  address.sun_family = AF_UNIX;
  strcpy(address.sun_path, socket_path.c_str());
  socklen_t address_length = SUN_LEN(&address);
#else
  struct sockaddr_in address {};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;

  address.sin_port = htons(port);

  socklen_t address_length = sizeof(address);
#endif

  // Attach to port
  if (bind(socket_holder->file_descriptor, (struct sockaddr *)&address, address_length) < 0) {
    throw std::runtime_error("bind failed");
  }
  if (listen(socket_holder->file_descriptor, max_connections) < 0) {
    throw std::runtime_error("listen failed");
  }

  // needed for GetNewClient()
  SocketSetNoneBlocking(socket_holder->file_descriptor);
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

  auto socket_byte_stream = std::make_shared<SocketByteStream>(
      socket_file_descriptor, connection_simulator_provider->make_incoming_connection_simulator(),
      connection_simulator_provider->make_outgoing_connection_simulator());

  socket_byte_stream->ConfigureSocket();

  return socket_byte_stream;
}
