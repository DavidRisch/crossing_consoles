#ifndef CROSSING_CONSOLES_I_CONNECTION_SIMULATOR_H
#define CROSSING_CONSOLES_I_CONNECTION_SIMULATOR_H

#include <cstdint>

/**
 * Used to simulate potential errors in a real world connection (e.g. UART).
 */
class IConnectionSimulator {
 public:
  virtual uint8_t Filter(uint8_t input) = 0;
};

#endif  // CROSSING_CONSOLES_I_CONNECTION_SIMULATOR_H
