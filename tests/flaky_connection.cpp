#include <gtest/gtest.h>

#include "../src/communication/ProtocolDefinition.h"
#include "../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../src/communication/connection_layer/connection/ClientSideConnectionManager.h"
#include "../src/communication/connection_layer/connection/ServerSideConnectionManager.h"
#include "../src/communication/connection_layer/event/PayloadEvent.h"
#include "fixtures/FlakyConnectionManagers.h"

TEST_F(FlakyConnectionManagers, AllPerfect) {
  run_test(false, false, false, false);
}

TEST_F(FlakyConnectionManagers, BadServerIncoming) {
  run_test(true, false, false, false);
}

TEST_F(FlakyConnectionManagers, BadServerOutgoing) {
  run_test(false, true, false, false);
}

TEST_F(FlakyConnectionManagers, BadServer) {
  run_test(true, true, false, false);
}

TEST_F(FlakyConnectionManagers, BadClientIncoming) {
  run_test(false, false, true, false);
}

TEST_F(FlakyConnectionManagers, BadClientOutgoing) {
  run_test(false, false, false, true);
}

TEST_F(FlakyConnectionManagers, BadClient) {
  run_test(false, false, true, true);
}

TEST_F(FlakyConnectionManagers, AllBad) {
  run_test(true, true, true, true);
}
