#include "SocketHolder.h"

#include <unistd.h>

#include <cassert>

SocketHolder::SocketHolder(file_descriptor_t file_descriptor)
    : file_descriptor(file_descriptor) {
  assert(file_descriptor != 0);
}

SocketHolder::~SocketHolder() {
  close(file_descriptor);
}
