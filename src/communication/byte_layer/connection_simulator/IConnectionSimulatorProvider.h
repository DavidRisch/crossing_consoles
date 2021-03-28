#ifndef CROSSING_CONSOLES_CONNECTION_SIMULATOR_PROVIDER_H
#define CROSSING_CONSOLES_CONNECTION_SIMULATOR_PROVIDER_H

#include <memory>

#include "IConnectionSimulator.h"

namespace communication::byte_layer {

/**
 * \brief Provides instances of `IConnectionSimulator`.
 */
class IConnectionSimulatorProvider {
 public:
  /**
   * Create a ConnectionSimulator to be used for incoming bytes.
   */
  virtual std::shared_ptr<IConnectionSimulator> make_incoming_connection_simulator() = 0;

  /**
   * Create a ConnectionSimulator to be used for outgoing bytes.
   */
  virtual std::shared_ptr<IConnectionSimulator> make_outgoing_connection_simulator() = 0;
};

}  // namespace communication::byte_layer

#endif  // CROSSING_CONSOLES_I_CONNECTION_SIMULATOR_H
