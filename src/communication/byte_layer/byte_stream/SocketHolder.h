#ifndef CROSSING_CONSOLES_SOCKET_HOLDER_H
#define CROSSING_CONSOLES_SOCKET_HOLDER_H

namespace communication {
namespace byte_layer {

typedef int file_descriptor_t;

/**
 * \brief Holds a file descriptor which is automatically closed in the destructor.
 */
class SocketHolder {
 public:
  const file_descriptor_t file_descriptor;

  explicit SocketHolder(file_descriptor_t file_descriptor);

  /**
   * \brief Closes the socket.
   */
  ~SocketHolder();
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_SOCKET_HOLDER_H
