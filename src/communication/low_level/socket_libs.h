#ifndef CROSSING_CONSOLES_SOCKET_LIBS_H
#define CROSSING_CONSOLES_SOCKET_LIBS_H

#ifdef _WIN32
// Windows
#include <winsock2.h>

#define ECONNREFUSED WSAECONNREFUSED

// SO_REUSEPORT does not exist on Windows
#define SO_REUSEPORT 0

#else
// Linux
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#endif

#endif  // CROSSING_CONSOLES_SOCKET_LIBS_H
