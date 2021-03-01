#include "SocketHolder.h"

#include <unistd.h>

#include <cassert>

using namespace communication;
using namespace communication;
using namespace communication::byte_layer;

SocketHolder::SocketHolder(file_descriptor_t file_descriptor)
    : file_descriptor(file_descriptor) {
  assert(file_descriptor != 0);
}

SocketHolder::~SocketHolder() {
  close(file_descriptor);
}
