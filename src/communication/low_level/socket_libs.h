#ifndef CROSSING_CONSOLES_SOCKET_LIBS_H
#define CROSSING_CONSOLES_SOCKET_LIBS_H

namespace socket {
#ifdef _WIN32
// Windows
#include <winsock2.h>

#else
// Linux
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

// required for INADDR_ANY macro to work with this namespace setup
#define in_addr_t socket::in_addr_t

#endif

}  // namespace socket

#endif  // CROSSING_CONSOLES_SOCKET_LIBS_H
