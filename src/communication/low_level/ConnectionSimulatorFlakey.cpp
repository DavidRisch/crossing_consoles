#include "ConnectionSimulatorFlakey.h"

#include <cstdlib>

ConnectionSimulatorFlakey::ConnectionSimulatorFlakey(double error_rate)
    : error_rate(error_rate) {
}

uint8_t ConnectionSimulatorFlakey::Filter(uint8_t input) {
  if (rand() % 100000 < error_rate * 100000) {
    return rand() % 256;
  } else {
    return input;
  }
}
