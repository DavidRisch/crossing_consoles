#include "ConnectionSimulatorPerfect.h"

uint8_t ConnectionSimulatorPerfect::Filter(uint8_t input) {
  return input;
}

ConnectionSimulatorPerfect ConnectionSimulatorPerfect::instance = ConnectionSimulatorPerfect();
