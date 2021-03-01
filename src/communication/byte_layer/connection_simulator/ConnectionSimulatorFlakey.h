#ifndef CROSSING_CONSOLES_CONNECTION_SIMULATOR_FLAKEY_H
#define CROSSING_CONSOLES_CONNECTION_SIMULATOR_FLAKEY_H

#include <cstdint>

#include "IConnectionSimulator.h"

/**
 * \brief Changes each byte to a random value with a probability of error_rate.
 */
class ConnectionSimulatorFlakey : public IConnectionSimulator {
 public:
  explicit ConnectionSimulatorFlakey(double error_rate);

  uint8_t Filter(uint8_t input) override;

 private:
  double error_rate;
};

#endif  // CROSSING_CONSOLES_CONNECTION_SIMULATOR_FLAKEY_H
