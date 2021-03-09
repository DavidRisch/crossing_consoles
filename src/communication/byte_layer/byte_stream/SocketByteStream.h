#ifndef CROSSING_CONSOLES_BYTE_STREAM_H
#define CROSSING_CONSOLES_BYTE_STREAM_H

#include <cstdint>
#include <memory>
#include <stdexcept>

#include "../connection_simulator/ConnectionSimulatorPerfect.h"
#include "IInputByteStream.h"
#include "IOutputByteStream.h"
#include "SocketHolder.h"

namespace communication {
namespace byte_layer {

typedef uint16_t port_t;

/// arbitrary chosen port number
inline const port_t socket_default_port = 56921;

/**
 * \brief Interface to communicate with another process without any structure to messages.
 */
class SocketByteStream : public IOutputByteStream, public IInputByteStream {
 public:
  explicit SocketByteStream(file_descriptor_t socket_file_descriptor,
                            IConnectionSimulator& connection_simulator_incoming = ConnectionSimulatorPerfect::instance,
                            IConnectionSimulator& connection_simulator_outgoing = ConnectionSimulatorPerfect::instance);

  /**
   * \brief Connect to a `SocketByteServer` and create a new `SocketByteStream`.
   * \details Blocks until a connection is established.
   */
  static std::shared_ptr<SocketByteStream> CreateClientSide(port_t port = socket_default_port);

  /**
   * \brief Configure the os socket.
   */
  void ConfigureSocket();

  /**
   * \brief Configure the os socket.
   */
  void ConfigureSocket();

  void Send(const uint8_t* send_buffer, size_t length) override;
  void SendString(const std::string& message);

  size_t Read(uint8_t* receive_buffer, size_t max_length) override;
  std::string ReadString(size_t max_length = 1024);

  /// Indicates data is available and can be read
  bool HasInput() override;

  void SetConnectionSimulatorIncoming(IConnectionSimulator& ConnectionSimulator);
  void SetConnectionSimulatorOutgoing(IConnectionSimulator& ConnectionSimulator);

  /**
   * \brief Thrown if a client tries to connect to a server which does not exist.
   */
  class ConnectionRefusedException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "Socket connection failed. Is the server started?";
    }
  };

 private:
  std::shared_ptr<SocketHolder> socket_holder;

  IConnectionSimulator* connection_simulator_incoming;
  IConnectionSimulator* connection_simulator_outgoing;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_BYTE_STREAM_H
