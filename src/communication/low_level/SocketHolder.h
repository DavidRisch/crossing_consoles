#ifndef CROSSING_CONSOLES_SOCKET_HOLDER_H
#define CROSSING_CONSOLES_SOCKET_HOLDER_H

typedef int file_descriptor_t;

/**
 * Holds a file descriptor which is automatically closed in the destructor.
 */
class SocketHolder {
 public:
  const file_descriptor_t file_descriptor;

  explicit SocketHolder(file_descriptor_t file_descriptor);
  ~SocketHolder();
};

#endif  // CROSSING_CONSOLES_SOCKET_HOLDER_H
