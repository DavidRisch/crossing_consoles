#include "ByteStream.h"

namespace socket {
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
}  // namespace socket

#include <cassert>
#include <cstring>

ByteStream ByteStream::CreateClientSide(uint16_t port) {
  file_descriptor_t socket_file_descriptor = socket::socket(AF_INET, socket::SOCK_STREAM, 0);
  if (socket_file_descriptor < 0) {
    throw std::runtime_error("socket::socket failed");
  }

  struct socket::sockaddr_in server_address {};
  server_address.sin_family = AF_INET;
  server_address.sin_port = socket::htons(port);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
    throw std::runtime_error("Invalid address");
  }

  if (socket::connect(socket_file_descriptor, (struct socket::sockaddr *)&server_address, sizeof(server_address)) < 0) {
    throw std::runtime_error("socket::connect failed");
  }

  return ByteStream(socket_file_descriptor);
}

size_t ByteStream::Read(  // NOLINT(readability-make-member-function-const)
    uint8_t *receive_buffer, size_t max_length) {
  ssize_t read_count = socket::recv(socket->file_descriptor, receive_buffer, max_length, 0);
  if (read_count < 0) {
    throw std::runtime_error("socket::recv failed");
  }
  assert(read_count <= max_length);
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
  ssize_t send_count = socket::send(socket->file_descriptor, send_buffer, length, 0);
  if (send_count < 0) {
    throw std::runtime_error("socket::send failed");
  }
}

void ByteStream::SendString(const std::string &message) {
  Send(reinterpret_cast<const uint8_t *>(message.c_str()), message.size());
}

ByteStream::ByteStream(file_descriptor_t socket_file_descriptor)
    : socket(std::make_shared<Socket>(socket_file_descriptor)) {
}
