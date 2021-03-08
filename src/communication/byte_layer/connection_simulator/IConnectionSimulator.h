#ifndef CROSSING_CONSOLES_I_CONNECTION_SIMULATOR_H
#define CROSSING_CONSOLES_I_CONNECTION_SIMULATOR_H

#include <cstdint>

namespace communication {
namespace byte_layer {

/**
 * \brief Used to simulate potential errors in a real world connection (e.g. UART).
 */
class IConnectionSimulator {
 public:
  /**
   * \brief Run byte through virtual connection and get end result.
   */
  virtual uint8_t Filter(uint8_t input) = 0;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_I_CONNECTION_SIMULATOR_H
