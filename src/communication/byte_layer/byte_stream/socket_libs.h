#ifndef CROSSING_CONSOLES_SOCKET_LIBS_H
#define CROSSING_CONSOLES_SOCKET_LIBS_H

/**
 * \file
 * \brief Includes all headers needed for sockets on Linux and Windows.
 */

#ifdef _WIN32
// Windows
#include <winsock2.h>
#include <ws2tcpip.h>

#define MSG_NOSIGNAL 0

#else
// Linux
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/un.h>

#endif

#ifdef USE_UNIX_SOCKET
const inline int SOCKET_PROTOCOL_FAMILY = PF_UNIX;
const inline char* SOCKET_FILE_PATH = "/tmp/crossing_consoles_socket";
#else
const inline int SOCKET_PROTOCOL_FAMILY = PF_INET;
#endif

inline void SocketSetNoneBlocking(int file_descriptor) {
#ifdef _WIN32
  // Windows
  u_long blocking_io_mode = 1;  // != 0 -> none blocking mode
  if (ioctlsocket(file_descriptor, FIONBIO, &blocking_io_mode) != 0) {
    throw std::runtime_error("ioctlsocket failed");
  }
#else
  // Linux
  if (fcntl(file_descriptor, F_SETFL, fcntl(file_descriptor, F_GETFL, 0) | O_NONBLOCK) != 0) {
    throw std::runtime_error("fcntl failed");
  }
#endif
}

#endif  // CROSSING_CONSOLES_SOCKET_LIBS_H
