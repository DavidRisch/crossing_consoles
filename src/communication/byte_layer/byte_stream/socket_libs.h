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

#endif

#endif  // CROSSING_CONSOLES_SOCKET_LIBS_H
