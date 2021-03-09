#include "../src/communication/connection_layer/Connection.h"

#include <gtest/gtest.h>

#include <thread>
#include <utility>

#include "../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../src/communication/connection_layer/ConnectionManager.h"
#include "../src/communication/message_layer/message/KeepAliveMessage.h"
#include "../src/communication/message_layer/message/PayloadMessage.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

void test_connection(std::shared_ptr<MessageInputStream> client_message_input_stream,
                     std::shared_ptr<MessageOutputStream> client_message_output_stream,
                     std::shared_ptr<MessageInputStream> server_message_input_stream,
                     std::shared_ptr<MessageOutputStream> server_message_output_stream) {
  std::shared_ptr<Connection> server_connection;

  std::thread server_thread([&server_connection, &server_message_input_stream, &server_message_output_stream] {
    server_connection = Connection::CreateServerSide(server_message_input_stream, server_message_output_stream);
  });

  auto client_connection =
      Connection::CreateClientSide(std::move(client_message_input_stream), std::move(client_message_output_stream));
  server_thread.join();

  address_t target_address = 1234;
  {
    KeepAliveMessage original_message(target_address);

    client_connection->SendMessage(&original_message);

    auto received_message = server_connection->ReceiveMessage();
    EXPECT_EQ(original_message.GetMessageType(), received_message->GetMessageType());
    auto ack_message = client_connection->ReceiveMessage();
    EXPECT_EQ(ack_message->GetMessageType(), MessageType::ACKNOWLEDGE);
  }

  {
    std::vector<uint8_t> payload;
    payload.push_back(123);
    payload.push_back(45);
    PayloadMessage original_message(target_address, payload);

    server_connection->SendMessage(&original_message);

    auto received_message = client_connection->ReceiveMessage();
    EXPECT_EQ(original_message.GetMessageType(), received_message->GetMessageType());
    auto &received_payload_message = dynamic_cast<PayloadMessage &>(*received_message);
    EXPECT_EQ(received_payload_message.GetPayload().size(), original_message.GetPayload().size());
    EXPECT_EQ(received_payload_message.GetPayload().at(0), original_message.GetPayload().at(0));
  }
}

TEST(Connection, Simple) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = stream_pair.first;
  auto &client_side = stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  test_connection(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                  server_message_output_stream);
}

TEST(Connection, Integration) {
  auto byte_server = std::make_shared<byte_layer::SocketByteServer>();

  auto client_byte_stream = byte_layer::SocketByteStream::CreateClientSide();
  auto client_message_input_stream = std::make_shared<message_layer::MessageInputStream>(client_byte_stream);
  auto client_message_output_stream = std::make_shared<message_layer::MessageOutputStream>(client_byte_stream);

  auto new_client = byte_server->GetNewClient();
  ASSERT_TRUE(new_client);
  auto server_message_input_stream = std::make_shared<message_layer::MessageInputStream>(new_client);
  auto server_message_output_stream = std::make_shared<message_layer::MessageOutputStream>(new_client);

  test_connection(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                  server_message_output_stream);
}

TEST(Connection, FailedHandshakeClient) {
  // Server unreachable, Client throws timeout exception
  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &client_side = stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  ASSERT_THROW(
      Connection::CreateClientSide(std::move(client_message_input_stream), std::move(client_message_output_stream)),
      ConnectionManager::ConnectionTimeout);
}

TEST(Connection, FailedHandshakeServer) {
  // Client unreachable, Server throws timeout exception
  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = stream_pair.second;

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  ASSERT_THROW(
      Connection::CreateServerSide(std::move(server_message_input_stream), std::move(server_message_output_stream)),
      ConnectionManager::ConnectionTimeout);
}
