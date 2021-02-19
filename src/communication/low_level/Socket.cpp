#include "Socket.h"

#include <unistd.h>

#include <cassert>

Socket::Socket(file_descriptor_t file_descriptor)
    : file_descriptor(file_descriptor) {
  assert(file_descriptor != 0);
}

Socket::~Socket() {
  close(file_descriptor);
}
