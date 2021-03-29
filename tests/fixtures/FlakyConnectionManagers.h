#ifndef CROSSING_CONSOLES_FLAKY_CONNECTION_MANAGERS_H
#define CROSSING_CONSOLES_FLAKY_CONNECTION_MANAGERS_H

#include <gtest/gtest.h>

#include <thread>

#include "../../src/communication/ProtocolDefinition.h"
#include "../../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../../src/communication/byte_layer/connection_simulator/ConnectionSimulatorFlaky.h"
#include "../../src/communication/connection_layer/connection/ClientSideConnectionManager.h"
#include "../../src/communication/connection_layer/connection/ServerSideConnectionManager.h"
#include "../../src/communication/connection_layer/event/PayloadEvent.h"
#include "../../src/communication/debug.h"
#include "../utils/detect_debugger.h"
#include "ConnectionManagers.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

class FlakyConnectionManagers : public ConnectionManagers {
 public:
  class FlakyConnectionSimulatorProvider : public IConnectionSimulatorProvider {
   public:
    explicit FlakyConnectionSimulatorProvider(ConnectionSimulatorFlaky::Parameters incoming_parameters,
                                              ConnectionSimulatorFlaky::Parameters outgoing_parameters)
        : incoming_parameters(incoming_parameters)
        , outgoing_parameters(outgoing_parameters) {
    }

    ConnectionSimulatorFlaky::Parameters incoming_parameters;
    ConnectionSimulatorFlaky::Parameters outgoing_parameters;

    std::shared_ptr<IConnectionSimulator> make_incoming_connection_simulator() override {
      return std::make_shared<ConnectionSimulatorFlaky>(incoming_parameters);
    }

    std::shared_ptr<communication::byte_layer::IConnectionSimulator> make_outgoing_connection_simulator() override {
      return std::make_shared<ConnectionSimulatorFlaky>(outgoing_parameters);
    }
  };

  void run_test(bool bad_server_incoming, bool bad_server_outgoing, bool bad_client_incoming,
                bool bad_client_outgoing) {
    for (int next_error = 0; next_error < test_until; ++next_error) {
      DEBUG_TESTS(std::cout << "\n ---> " << next_error << "\n")

      auto server_incoming_parameters =
          std::make_unique<ConnectionSimulatorFlaky::Parameters>(ConnectionSimulatorFlaky::Parameters::Perfect());
      if (bad_server_incoming) {
        server_incoming_parameters =
            std::make_unique<ConnectionSimulatorFlaky::Parameters>(ConnectionSimulatorFlaky::Parameters(next_error));
      }

      auto server_outgoing_parameters =
          std::make_unique<ConnectionSimulatorFlaky::Parameters>(ConnectionSimulatorFlaky::Parameters::Perfect());
      if (bad_server_outgoing) {
        server_outgoing_parameters =
            std::make_unique<ConnectionSimulatorFlaky::Parameters>(ConnectionSimulatorFlaky::Parameters(next_error));
      }

      auto client_incoming_parameters =
          std::make_unique<ConnectionSimulatorFlaky::Parameters>(ConnectionSimulatorFlaky::Parameters::Perfect());
      if (bad_client_incoming) {
        client_incoming_parameters =
            std::make_unique<ConnectionSimulatorFlaky::Parameters>(ConnectionSimulatorFlaky::Parameters(next_error));
      }

      auto client_outgoing_parameters =
          std::make_unique<ConnectionSimulatorFlaky::Parameters>(ConnectionSimulatorFlaky::Parameters::Perfect());
      if (bad_client_outgoing) {
        client_outgoing_parameters =
            std::make_unique<ConnectionSimulatorFlaky::Parameters>(ConnectionSimulatorFlaky::Parameters(next_error));
      }

      server_connection_simulator_provider.reset(
          new FlakyConnectionSimulatorProvider(*server_incoming_parameters, *server_outgoing_parameters));
      client_connection_simulator_provider.reset(
          new FlakyConnectionSimulatorProvider(*client_incoming_parameters, *client_outgoing_parameters));

      create_server_and_client();
      send_and_check_messages(10);

      destroy();
    }
  }

  static const int test_until = 50;
};

#endif
