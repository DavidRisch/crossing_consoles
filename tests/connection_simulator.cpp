#include <gtest/gtest.h>

#include <thread>

#include "../src/communication/low_level/ByteServer.h"
#include "../src/communication/low_level/ConnectionSimulatorFlakey.h"

void test_with_connection_simulators(const char *server_to_client, const char *client_to_server,
                                     std::string &server_received, std::string &client_received,
                                     IConnectionSimulator &server_incoming, IConnectionSimulator &server_outgoing,
                                     IConnectionSimulator &client_incoming, IConnectionSimulator &client_outgoing) {
  std::thread server_thread([server_to_client, &server_received, &server_incoming, &server_outgoing] {
    ByteServer byte_server;
    std::optional<ByteStream> byte_stream;

    int counter = 100;
    while (!byte_stream.has_value() && counter > 0) {
      byte_stream = byte_server.GetNewClient();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      counter--;
    }

    ASSERT_NE(counter, 0);
    ASSERT_TRUE(byte_stream.has_value());

    byte_stream->SetConnectionSimulatorIncoming(server_incoming);
    byte_stream->SetConnectionSimulatorOutgoing(server_outgoing);

    byte_stream->SendString(server_to_client);
    server_received = byte_stream->ReadString();
  });

  std::thread client_thread([client_to_server, &client_received, &client_incoming, &client_outgoing] {
    ByteStream byte_stream = ByteStream::CreateClientSide();

    byte_stream.SetConnectionSimulatorIncoming(client_incoming);
    byte_stream.SetConnectionSimulatorOutgoing(client_outgoing);

    byte_stream.SendString(client_to_server);
    client_received = byte_stream.ReadString();
  });

  server_thread.join();
  client_thread.join();
}

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
};

TEST_F(ConnectionSimulator, AllPerfect) {
  test_with_connection_simulators(server_to_client, client_to_server, server_received, client_received,  //
                                  perfect, perfect, perfect, perfect);
  EXPECT_EQ(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, AllFlakeyPerfect) {
  test_with_connection_simulators(server_to_client, client_to_server, server_received, client_received,  //
                                  flakey_perfect, flakey_perfect, flakey_perfect, flakey_perfect);
  EXPECT_EQ(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, FlakeyServerIncoming) {
  test_with_connection_simulators(server_to_client, client_to_server, server_received, client_received,  //
                                  flakey, perfect, perfect, perfect);
  EXPECT_EQ(server_to_client, client_received);
  EXPECT_NE(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, FlakeyServerOutgoing) {
  test_with_connection_simulators(server_to_client, client_to_server, server_received, client_received,  //
                                  perfect, flakey, perfect, perfect);
  EXPECT_NE(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, FlakeyClientIncoming) {
  test_with_connection_simulators(server_to_client, client_to_server, server_received, client_received,  //
                                  perfect, perfect, flakey, perfect);
  EXPECT_NE(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, FlakeyClientOutgoing) {
  test_with_connection_simulators(server_to_client, client_to_server, server_received, client_received,  //
                                  perfect, perfect, perfect, flakey);
  EXPECT_EQ(server_to_client, client_received);
  EXPECT_NE(client_to_server, server_received);
}

TEST_F(ConnectionSimulator, AllFlakey) {
  test_with_connection_simulators(server_to_client, client_to_server, server_received, client_received,  //
                                  flakey, flakey, flakey, flakey);
  EXPECT_NE(server_to_client, client_received);
  EXPECT_NE(client_to_server, server_received);
}
