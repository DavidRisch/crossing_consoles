#include "ConnectionSimulatorFlaky.h"

#include <cstdlib>

using namespace communication;
using namespace communication::byte_layer;

ConnectionSimulatorFlaky::ConnectionSimulatorFlaky(double error_rate)
    : error_rate(error_rate) {
}

uint8_t ConnectionSimulatorFlaky::Filter(uint8_t input) {
  if (rand() % 100000 < error_rate * 100000) {
    return static_cast<uint8_t>(rand() % 256);
  } else {
    return input;
  }
}
