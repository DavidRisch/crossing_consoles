#include <gtest/gtest.h>

#include <thread>

#include "../src/communication/low_level/ByteServer.h"

void test_single_client() {
  const char* server_to_client = "0123456789";
  const char* client_to_server = "ABCDEFGHIJ";

  std::string client_received;

  ByteServer byte_server;

  std::thread client_thread([client_to_server, &client_received] {
    ByteStream b = ByteStream::CreateClientSide();
    b.SendString(client_to_server);
    client_received = b.ReadString();
  });

  std::optional<ByteStream> byte_stream;

  int counter = 1000;
  while (!byte_stream.has_value() && counter > 0) {
    byte_stream = byte_server.GetNewClient();
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    counter--;
  }

  ASSERT_NE(counter, 0);
  ASSERT_TRUE(byte_stream.has_value());

  byte_stream->SendString(server_to_client);
  std::string server_received = byte_stream->ReadString();

  client_thread.join();

  EXPECT_EQ(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST(ByteServer, SingleClient) {
  test_single_client();
}

TEST(ByteServer, TwoClients) {
  const char* server_to_client1 = "1abc123";
  const char* server_to_client2 = "2abc123";
  const char* client1_to_server = "123ABC1";
  const char* client2_to_server = "123ABC2";

  std::string server_received;
  std::string client1_received;
  std::string client2_received;

  ByteServer byte_server;

  std::thread server_thread([&byte_server, server_to_client1, server_to_client2, &server_received] {
    std::optional<ByteStream> byte_stream;

    int success_count = 0;
    const char* next_message = server_to_client1;

    int counter = 1000;
    while (success_count < 2 && counter > 0) {
      byte_stream = byte_server.GetNewClient();
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      counter--;
      if (byte_stream.has_value()) {
        byte_stream->SendString(next_message);
        next_message = server_to_client2;
        server_received += byte_stream->ReadString();
        success_count++;
      }
    }

    ASSERT_NE(counter, 0);
  });

  std::thread client1_thread([client1_to_server, &client1_received] {
    ByteStream b = ByteStream::CreateClientSide();
    b.SendString(client1_to_server);
    client1_received = b.ReadString();
  });

  client1_thread.join();
  EXPECT_EQ(server_to_client1, client1_received);
  EXPECT_EQ(std::string(client1_to_server), server_received);

  std::thread client2_thread([client2_to_server, &client2_received] {
    ByteStream b = ByteStream::CreateClientSide();
    b.SendString(client2_to_server);
    client2_received = b.ReadString();
  });

  client2_thread.join();
  server_thread.join();

  EXPECT_EQ(server_to_client2, client2_received);
  EXPECT_EQ(std::string(client1_to_server) + client2_to_server, server_received);
}
