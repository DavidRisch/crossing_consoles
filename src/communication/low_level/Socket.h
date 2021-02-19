#ifndef CROSSING_CONSOLES_SOCKET_H
#define CROSSING_CONSOLES_SOCKET_H

typedef int file_descriptor_t;

/**
 * Holds a file descriptor which is automatically closed in the destructor.
 */
class Socket {
 public:
  const file_descriptor_t file_descriptor;

  explicit Socket(file_descriptor_t file_descriptor);
  ~Socket();
};

#endif  // CROSSING_CONSOLES_SOCKET_H
