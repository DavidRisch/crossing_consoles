#include "ConnectionSimulatorPerfect.h"

using namespace communication;
using namespace communication::byte_layer;

uint8_t ConnectionSimulatorPerfect::Filter(uint8_t input) {
  return input;
}

ConnectionSimulatorPerfect ConnectionSimulatorPerfect::instance = ConnectionSimulatorPerfect();
