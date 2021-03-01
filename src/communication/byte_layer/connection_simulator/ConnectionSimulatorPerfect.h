#ifndef CROSSING_CONSOLES_CONNECTION_SIMULATOR_PERFECT_H
#define CROSSING_CONSOLES_CONNECTION_SIMULATOR_PERFECT_H

#include <cstdint>

#include "IConnectionSimulator.h"

namespace communication {
namespace byte_layer {

/**
 * \brief Does not change any bytes.
 */
class ConnectionSimulatorPerfect : public IConnectionSimulator {
 public:
  uint8_t Filter(uint8_t input) override;

  /**
   * \brief Required for the constructor of ByteStream.
   * \details Because this class holds no state this is ok here.
   */
  static ConnectionSimulatorPerfect instance;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTION_SIMULATOR_PERFECT_H
