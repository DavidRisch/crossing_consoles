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
  /**
   * \brief Holds the parameters required to make a `ConnectionSimulatorFlaky`.
   */
  class Parameters {
   public:
    explicit Parameters(int first_error, int error_interval = -1, int error_length = 1)
        : first_error(first_error)
        , error_interval(error_interval)
        , error_length(error_length) {
    }

    /// Number of bytes until the first error should be generated.
    int first_error;

    /// Distance in bytes between the start of two errors.
    int error_interval;

    /// Length in bytes of each error.
    int error_length;

    static Parameters Perfect() {
      return Parameters(-1);
    }
  };

  explicit ConnectionSimulatorFlaky(Parameters parameters);

  uint8_t Filter(uint8_t input) override;

 private:
  Parameters parameters;
  int next_error;
  int current_error_length = 0;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTION_SIMULATOR_FLAKY_H
