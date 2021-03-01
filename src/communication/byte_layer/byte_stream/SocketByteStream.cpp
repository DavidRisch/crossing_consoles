#include "SocketByteStream.h"

#include <cassert>
#include <climits>
#include <cstring>

#include "socket_libs.h"

using namespace communication;
using namespace communication::byte_layer;

SocketByteStream::SocketByteStream(file_descriptor_t socket_file_descriptor,
                                   IConnectionSimulator &connection_simulator_incoming,
                                   IConnectionSimulator &connection_simulator_outgoing)
    : socket_holder(std::make_shared<SocketHolder>(socket_file_descriptor))
    , connection_simulator_incoming(&connection_simulator_incoming)
    , connection_simulator_outgoing(&connection_simulator_outgoing) {
}

SocketByteStream SocketByteStream::CreateClientSide(uint16_t port) {
  file_descriptor_t socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_file_descriptor < 0) {
    throw std::runtime_error("socket failed");
  }

  struct sockaddr_in server_address {};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);

  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(socket_file_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    if (errno == ECONNREFUSED) {
      throw ConnectionRefusedException();
    }
    throw std::runtime_error("connect failed");
  }

  return SocketByteStream(socket_file_descriptor);
}

size_t SocketByteStream::Read(  // NOLINT(readability-make-member-function-const)
    uint8_t *receive_buffer, size_t max_length) {
  assert(max_length < SSIZE_MAX);
  ssize_t read_count = recv(socket_holder->file_descriptor, reinterpret_cast<char *>(receive_buffer), max_length, 0);
  if (read_count < 0) {
    throw std::runtime_error("recv failed");
  }
  assert(static_cast<size_t>(read_count) <= max_length);

  for (ssize_t i = 0; i < read_count; ++i) {
    receive_buffer[i] = connection_simulator_incoming->Filter(receive_buffer[i]);
  }

  return static_cast<size_t>(read_count);
}

std::string SocketByteStream::ReadString(size_t max_length) {
  char *receive_buffer = new char[max_length + 1];
  auto read_count = Read(reinterpret_cast<uint8_t *>(receive_buffer), max_length);
  receive_buffer[read_count] = '\0';
  std::string received(receive_buffer);
  delete[] receive_buffer;
  return received;
}

void SocketByteStream::Send(  // NOLINT(readability-make-member-function-const)
    const uint8_t *send_buffer, size_t length) {
  auto *modified_send_buffer = new uint8_t[length];

  for (size_t i = 0; i < length; ++i) {
    modified_send_buffer[i] = connection_simulator_outgoing->Filter(send_buffer[i]);
  }

  ssize_t send_count = send(socket_holder->file_descriptor, reinterpret_cast<char *>(modified_send_buffer), length, 0);

  delete[] modified_send_buffer;

  if (send_count < 0) {
    throw std::runtime_error("send failed");
  }
}

void SocketByteStream::SendString(const std::string &message) {
  Send(reinterpret_cast<const uint8_t *>(message.c_str()), message.size());
}

void SocketByteStream::SetConnectionSimulatorIncoming(IConnectionSimulator &ConnectionSimulator) {
  connection_simulator_incoming = &ConnectionSimulator;
}

void SocketByteStream::SetConnectionSimulatorOutgoing(IConnectionSimulator &ConnectionSimulator) {
  connection_simulator_outgoing = &ConnectionSimulator;
}
