#ifndef CROSSING_CONSOLES_CONNECTION_SIMULATOR_PERFECT_H
#define CROSSING_CONSOLES_CONNECTION_SIMULATOR_PERFECT_H

#include <cstdint>

#include "IConnectionSimulator.h"

/**
 * Does not change any bytes.
 */
class ConnectionSimulatorPerfect : public IConnectionSimulator {
 public:
  uint8_t Filter(uint8_t input) override;

  /**
   * Required for the constructor of ByteStream.
   * Because this class holds no state this is ok here.
   */
  static ConnectionSimulatorPerfect instance;
};

#endif  // CROSSING_CONSOLES_CONNECTION_SIMULATOR_PERFECT_H
