#include "ByteStream.h"

#include <cassert>
#include <cstring>

#include "socket_libs.h"

ByteStream::ByteStream(file_descriptor_t socket_file_descriptor, IConnectionSimulator &connection_simulator_incoming,
                       IConnectionSimulator &connection_simulator_outgoing)
    : socket_ptr(std::make_shared<Socket>(socket_file_descriptor))
    , connection_simulator_incoming(&connection_simulator_incoming)
    , connection_simulator_outgoing(&connection_simulator_outgoing) {
}

ByteStream ByteStream::CreateClientSide(uint16_t port) {
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

  return ByteStream(socket_file_descriptor);
}

size_t ByteStream::Read(  // NOLINT(readability-make-member-function-const)
    uint8_t *receive_buffer, size_t max_length) {
  ssize_t read_count = recv(socket_ptr->file_descriptor, reinterpret_cast<char *>(receive_buffer), max_length, 0);
  if (read_count < 0) {
    throw std::runtime_error("recv failed");
  }
  assert(read_count <= max_length);

  for (int i = 0; i < read_count; ++i) {
    receive_buffer[i] = connection_simulator_incoming->Filter(receive_buffer[i]);
  }

  return static_cast<size_t>(read_count);
}

std::string ByteStream::ReadString(size_t max_length) {
  char *receive_buffer = new char[max_length + 1];
  Read(reinterpret_cast<uint8_t *>(receive_buffer), max_length);
  std::string received(receive_buffer);
  delete[] receive_buffer;
  return received;
}

void ByteStream::Send(  // NOLINT(readability-make-member-function-const)
    const uint8_t *send_buffer, size_t length) {
  auto *modified_send_buffer = new uint8_t[length];

  for (int i = 0; i < length; ++i) {
    modified_send_buffer[i] = connection_simulator_outgoing->Filter(send_buffer[i]);
  }

  ssize_t send_count = send(socket_ptr->file_descriptor, reinterpret_cast<char *>(modified_send_buffer), length, 0);

  delete[] modified_send_buffer;

  if (send_count < 0) {
    throw std::runtime_error("send failed");
  }
}

void ByteStream::SendString(const std::string &message) {
  Send(reinterpret_cast<const uint8_t *>(message.c_str()), message.size());
}

void ByteStream::SetConnectionSimulatorIncoming(IConnectionSimulator &ConnectionSimulator) {
  connection_simulator_incoming = &ConnectionSimulator;
}

void ByteStream::SetConnectionSimulatorOutgoing(IConnectionSimulator &ConnectionSimulator) {
  connection_simulator_outgoing = &ConnectionSimulator;
}
