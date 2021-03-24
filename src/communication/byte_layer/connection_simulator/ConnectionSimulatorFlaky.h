#ifndef CROSSING_CONSOLES_CONNECTION_SIMULATOR_FLAKY_H
#define CROSSING_CONSOLES_CONNECTION_SIMULATOR_FLAKY_H

#include <cstdint>

#include "IConnectionSimulator.h"

namespace communication {
namespace byte_layer {

/**
 * \brief Changes each byte to a random value with a probability of error_rate.
 */
class ConnectionSimulatorFlaky : public IConnectionSimulator {
 public:
  explicit ConnectionSimulatorFlaky(double error_rate);

  uint8_t Filter(uint8_t input) override;

 private:
  double error_rate;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTION_SIMULATOR_FLAKY_H
