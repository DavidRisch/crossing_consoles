#include "ConnectionSimulatorPerfect.h"

using namespace communication;
using namespace communication::byte_layer;

uint8_t ConnectionSimulatorPerfect::Filter(uint8_t input) {
  return input;
}

const std::shared_ptr<ConnectionSimulatorPerfect> ConnectionSimulatorPerfect::instance =
    std::make_shared<ConnectionSimulatorPerfect>();
