#include <gtest/gtest.h>

#include <thread>

#include "../src/communication/low_level/ByteServer.h"

TEST(ByteServer, SingleClient) {
  const char* server_to_client = "abc123";
  const char* client_to_server = "123ABC";

  std::string server_received;
  std::string client_received;

  std::thread server_thread([server_to_client, &server_received] {
    ByteServer byte_server;
    std::optional<ByteStream> byte_stream;

    int counter = 100;
    while (!byte_stream.has_value() && counter > 0) {
      byte_stream = byte_server.GetNewClient();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      counter--;
    }

    ASSERT_LT(counter, 100);
    ASSERT_NE(counter, 0);
    ASSERT_TRUE(byte_stream.has_value());

    byte_stream->SendString(server_to_client);
    server_received = byte_stream->ReadString();
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(2));

  std::thread client_thread([client_to_server, &client_received] {
    ByteStream b = ByteStream::CreateClientSide();
    b.SendString(client_to_server);
    client_received = b.ReadString();
  });

  server_thread.join();
  client_thread.join();

  EXPECT_EQ(server_to_client, client_received);
  EXPECT_EQ(client_to_server, server_received);
}

TEST(ByteServer, TwoClients) {
  const char* server_to_client1 = "1abc123";
  const char* server_to_client2 = "2abc123";
  const char* client1_to_server = "123ABC1";
  const char* client2_to_server = "123ABC2";

  std::string server_received;
  std::string client1_received;
  std::string client2_received;

  std::thread server_thread([server_to_client1, server_to_client2, &server_received] {
    ByteServer byte_server;
    std::optional<ByteStream> byte_stream;

    int success_count = 0;
    const char* next_message = server_to_client1;

    int counter = 100;
    while (success_count < 2 && counter > 0) {
      byte_stream = byte_server.GetNewClient();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
