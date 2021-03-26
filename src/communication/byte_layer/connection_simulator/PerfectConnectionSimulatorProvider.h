#ifndef CROSSING_CONSOLES_CONNECTION_SIMULATOR_PROVIDER_PERFECT_H
#define CROSSING_CONSOLES_CONNECTION_SIMULATOR_PROVIDER_PERFECT_H

#include <memory>

#include "IConnectionSimulatorProvider.h"

namespace communication::byte_layer {

/**
 * \brief Provides `ConnectionSimulatorPerfect` for incoming and outgoing bytes.
 */
class PerfectConnectionSimulatorProvider : public IConnectionSimulatorProvider {
 public:
  virtual std::shared_ptr<IConnectionSimulator> make_incoming_connection_simulator();
  virtual std::shared_ptr<IConnectionSimulator> make_outgoing_connection_simulator();

  /**
   * \brief Required for the constructor of ConnectionManager.
   * \details Because this class holds no state this is ok here.
   */
  static const std::shared_ptr<PerfectConnectionSimulatorProvider> instance;
};

}  // namespace communication::byte_layer

#endif  // CROSSING_CONSOLES_CONNECTION_SIMULATOR_PROVIDER_PERFECT_H
