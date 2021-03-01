#include <gtest/gtest.h>

#include <thread>

#include "../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../src/communication/byte_layer/connection_simulator/ConnectionSimulatorFlakey.h"

class ConnectionSimulator : public ::testing::Test {
 public:
  ConnectionSimulator()
      : flakey_perfect(0.0)
      , flakey(0.5) {
  }

  ConnectionSimulatorPerfect perfect;
  ConnectionSimulatorFlakey flakey_perfect;
  ConnectionSimulatorFlakey flakey;

  const char *server_to_client = "abc123abc123abc123abc123abc123abc123abc123abc123";
  const char *client_to_server = "123ABC123ABC123ABC123ABC123ABC123ABC123ABC123ABC";

  std::string server_received;
  std::string client_received;

  void TestWithConnectionSimulators(IConnectionSimulator &server_incoming, IConnectionSimulator &server_outgoing,
                                    IConnectionSimulator &client_incoming, IConnectionSimulator &client_outgoing) {
    SocketByteServer byte_server;

    std::thread client_thread([this, &client_incoming, &client_outgoing] {
      SocketByteStream byte_stream = SocketByteStream::CreateClientSide();

      byte_stream.SetConnectionSimulatorIncoming(client_incoming);
      byte_stream.SetConnectionSimulatorOutgoing(client_outgoing);

      byte_stream.SendString(client_to_server);
      client_received = byte_stream.ReadString();
    });

    std::optional<SocketByteStream> byte_stream;

    int counter = 1000;
    while (!byte_stream.has_value() && counter > 0) {
      byte_stream = byte_server.GetNewClient();
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      counter--;
    }

    ASSERT_NE(counter, 0);
    ASSERT_TRUE(byte_stream.has_value());

    byte_stream->SetConnectionSimulatorIncoming(server_incoming);
    byte_stream->SetConnectionSimulatorOutgoing(server_outgoing);

    byte_stream->SendString(server_to_client);
    server_received = byte_stream->ReadString();

    client_thread.join();
  }
};

TEST_F(ConnectionSimulator, AllPerfect) {
  TestWithConnectionSimulators(perfect, perfect, perfect, perfect);
  EXPECT_EQ(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, AllFlakeyPerfect) {
  TestWithConnectionSimulators(flakey_perfect, flakey_perfect, flakey_perfect, flakey_perfect);
  EXPECT_EQ(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, FlakeyServerIncoming) {
  TestWithConnectionSimulators(flakey, perfect, perfect, perfect);
  EXPECT_EQ(server_to_client, client_received);
  EXPECT_NE(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, FlakeyServerOutgoing) {
  TestWithConnectionSimulators(perfect, flakey, perfect, perfect);
  EXPECT_NE(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, FlakeyClientIncoming) {
  TestWithConnectionSimulators(perfect, perfect, flakey, perfect);
  EXPECT_NE(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, FlakeyClientOutgoing) {
  TestWithConnectionSimulators(perfect, perfect, perfect, flakey);
  EXPECT_EQ(server_to_client, client_received);
  EXPECT_NE(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, AllFlakey) {
  TestWithConnectionSimulators(flakey, flakey, flakey, flakey);
  EXPECT_NE(server_to_client, client_received);
  EXPECT_NE(client_to_server, server_received);
}
