#include "ConnectionSimulatorFlaky.h"

using namespace communication;
using namespace communication::byte_layer;

ConnectionSimulatorFlaky::ConnectionSimulatorFlaky(ConnectionSimulatorFlaky::Parameters parameters)
    : parameters(parameters)
    , next_error(parameters.first_error) {
}

uint8_t ConnectionSimulatorFlaky::Filter(uint8_t input) {
  if (next_error == -1) {
    return input;
  }

  if (next_error == 0) {
    current_error_length++;
    if (current_error_length >= parameters.error_length) {
      // last byte of current error

      current_error_length = 0;
      if (parameters.error_interval != 0) {
        next_error = parameters.error_interval - parameters.error_length;
      } else {
        next_error = -1;
      }
    }

    return input ^ 0xffu;
  } else {
    next_error--;
    return input;
  }
}
