#ifndef CROSSING_CONSOLES_BYTE_STREAM_H
#define CROSSING_CONSOLES_BYTE_STREAM_H

#include <cstdint>
#include <memory>
#include <stdexcept>

#include "ConnectionSimulatorPerfect.h"
#include "Socket.h"

typedef int file_descriptor_t;
typedef uint16_t port_t;

/// randomly chosen port number
inline const port_t socket_default_port = 56921;

/**
 * Interface to communicate with another process without any structure to messages.
 */
class ByteStream {
 public:
  explicit ByteStream(file_descriptor_t socket_file_descriptor,
                      IConnectionSimulator& connection_simulator_incoming = ConnectionSimulatorPerfect::instance,
                      IConnectionSimulator& connection_simulator_outgoing = ConnectionSimulatorPerfect::instance);

  static ByteStream CreateClientSide(port_t port = socket_default_port);

  void Send(const uint8_t* send_buffer, size_t length);
  void SendString(const std::string& message);

  size_t Read(uint8_t* receive_buffer, size_t max_length);
  std::string ReadString(size_t max_length = 1024);

  void SetConnectionSimulatorIncoming(IConnectionSimulator& ConnectionSimulator);
  void SetConnectionSimulatorOutgoing(IConnectionSimulator& ConnectionSimulator);

 private:
  std::shared_ptr<Socket> socket;

  IConnectionSimulator* connection_simulator_incoming;
  IConnectionSimulator* connection_simulator_outgoing;
};

#endif  // CROSSING_CONSOLES_BYTE_STREAM_H
