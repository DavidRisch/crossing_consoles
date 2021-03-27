
#include "PerfectConnectionSimulatorProvider.h"

#include "ConnectionSimulatorPerfect.h"

using namespace communication;
using namespace communication::byte_layer;

std::shared_ptr<IConnectionSimulator>
communication::byte_layer::PerfectConnectionSimulatorProvider::make_incoming_connection_simulator() {
  return ConnectionSimulatorPerfect::instance;
}

std::shared_ptr<communication::byte_layer::IConnectionSimulator>
communication::byte_layer::PerfectConnectionSimulatorProvider::make_outgoing_connection_simulator() {
  return ConnectionSimulatorPerfect::instance;
}

const std::shared_ptr<PerfectConnectionSimulatorProvider> PerfectConnectionSimulatorProvider::instance =
    std::make_shared<PerfectConnectionSimulatorProvider>();
