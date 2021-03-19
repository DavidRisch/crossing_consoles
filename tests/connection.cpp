#include "../src/communication/connection_layer/connection/Connection.h"

#include <gtest/gtest.h>

#include <thread>
#include <utility>

#include "../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../src/communication/byte_layer/byte_stream/SocketByteServer.h"
#include "../src/communication/connection_layer/connection/ConnectionManager.h"
#include "../src/communication/message_layer/message/AcknowledgeMessage.h"
#include "../src/communication/message_layer/message/KeepAliveMessage.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

class Connections : public ::testing::Test {
 public:
  Connections() = default;

  std::shared_ptr<Connection> server_connection;
  std::shared_ptr<Connection> client_connection;

  void make_connections(std::shared_ptr<MessageInputStream> client_message_input_stream,
                        std::shared_ptr<MessageOutputStream> client_message_output_stream,
                        std::shared_ptr<MessageInputStream> server_message_input_stream,
                        std::shared_ptr<MessageOutputStream> server_message_output_stream) {
    server_connection =
        Connection::CreateServerSide(std::move(server_message_input_stream), std::move(server_message_output_stream));

    std::thread server_thread([this] { server_connection->BlockingEstablish(); });

    client_connection =
        Connection::CreateClientSide(std::move(client_message_input_stream), std::move(client_message_output_stream));

    client_connection->BlockingEstablish();
    server_thread.join();
  }

  void test_connections() {
    {
      auto original_message = std::make_shared<KeepAliveMessage>();

      client_connection->SendMessage(original_message);

      auto received_message = server_connection->ReceiveMessage();
      ASSERT_NE(received_message, nullptr);
      EXPECT_EQ(original_message->GetMessageType(), received_message->GetMessageType());

      auto ack_message = client_connection->ReceiveMessage();
      ASSERT_NE(ack_message, nullptr);
      EXPECT_EQ(ack_message->GetMessageType(), MessageType::ACKNOWLEDGE);
    }

    {
      std::vector<uint8_t> payload;
      payload.push_back(123);
      payload.push_back(45);
      auto original_message = std::make_shared<PayloadMessage>(payload);

      server_connection->SendMessage(original_message);

      auto received_message = client_connection->ReceiveMessage();
      ASSERT_NE(received_message, nullptr);
      EXPECT_EQ(original_message->GetMessageType(), received_message->GetMessageType());
      auto &received_payload_message = dynamic_cast<PayloadMessage &>(*received_message);
      EXPECT_EQ(received_payload_message.GetPayload().size(), original_message->GetPayload().size());
      EXPECT_EQ(received_payload_message.GetPayload().at(0), original_message->GetPayload().at(0));

      auto ack_message = server_connection->ReceiveMessage();
      ASSERT_NE(ack_message, nullptr);
      EXPECT_EQ(ack_message->GetMessageType(), MessageType::ACKNOWLEDGE);
    }
  }
};

TEST_F(Connections, Simple) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = stream_pair.first;
  auto &client_side = stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  make_connections(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                   server_message_output_stream);
  test_connections();
}

TEST_F(Connections, Integration) {
  auto byte_server = std::make_shared<byte_layer::SocketByteServer>();

  auto client_byte_stream = byte_layer::SocketByteStream::CreateClientSide();
  auto client_message_input_stream = std::make_shared<message_layer::MessageInputStream>(client_byte_stream);
  auto client_message_output_stream = std::make_shared<message_layer::MessageOutputStream>(client_byte_stream);

  auto new_client = byte_server->GetNewClient();
  ASSERT_TRUE(new_client);
  auto server_message_input_stream = std::make_shared<message_layer::MessageInputStream>(new_client);
  auto server_message_output_stream = std::make_shared<message_layer::MessageOutputStream>(new_client);

  make_connections(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                   server_message_output_stream);
  test_connections();
}

TEST_F(Connections, FailedHandshakeClient) {
  // Server unreachable, Client throws timeout exception

  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &client_side = stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  auto connection = Connection::CreateClientSide(std::move(client_message_input_stream),
                                                 std::move(client_message_output_stream), std::chrono::milliseconds(1));

  ASSERT_THROW(connection->BlockingEstablish(), ConnectionManager::TimeoutException);
}

TEST_F(Connections, FailedHandshakeServer) {
  // Client unreachable, Server throws timeout exception

  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = stream_pair.second;

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  auto connection = Connection::CreateServerSide(std::move(server_message_input_stream),
                                                 std::move(server_message_output_stream), std::chrono::milliseconds(1));

  ASSERT_THROW(connection->BlockingEstablish(), ConnectionManager::TimeoutException);
}

TEST_F(Connections, SendQueue) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = stream_pair.first;
  auto &client_side = stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  make_connections(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                   server_message_output_stream);

  int count = 20;
  {
    auto original_message = std::make_shared<KeepAliveMessage>();

    for (int i = 0; i < count; ++i) {
      client_connection->SendMessage(original_message);
    }

    for (int i = 0; i < count; ++i) {
      auto received_message = server_connection->ReceiveMessage();
      ASSERT_NE(received_message, nullptr);
      EXPECT_EQ(original_message->GetMessageType(), received_message->GetMessageType());

      auto ack_message = client_connection->ReceiveMessage();
      EXPECT_EQ(ack_message->GetMessageType(), MessageType::ACKNOWLEDGE);

      // send next message from queue
      client_connection->Handle();
    }
  }

  {
    std::vector<std::shared_ptr<PayloadMessage>> messages;

    for (int i = 0; i < count; ++i) {
      std::vector<uint8_t> payload;
      payload.push_back(123);
      payload.push_back(i);
      payload.push_back(45);
      auto original_message = std::make_shared<PayloadMessage>(payload);

      messages.push_back(original_message);
    }

    for (const auto &original_message : messages) {
      server_connection->SendMessage(original_message);
    }

    for (const auto &original_message : messages) {
      auto received_message = client_connection->ReceiveMessage();
      ASSERT_NE(received_message, nullptr);
      EXPECT_EQ(original_message->GetMessageType(), received_message->GetMessageType());
      auto &received_payload_message = dynamic_cast<PayloadMessage &>(*received_message);
      EXPECT_EQ(received_payload_message.GetPayload().size(), original_message->GetPayload().size());
      EXPECT_EQ(received_payload_message.GetPayload().at(0), original_message->GetPayload().at(0));

      auto ack_message = server_connection->ReceiveMessage();
      EXPECT_EQ(ack_message->GetMessageType(), MessageType::ACKNOWLEDGE);

      // send next message from queue
      server_connection->Handle();
    }
  }
}

TEST_F(Connections, BadAcknowledgeException) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = stream_pair.first;
  auto &client_side = stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  make_connections(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                   server_message_output_stream);

  auto message = std::make_shared<AcknowledgeMessage>(0, 1234);

  server_connection->SendMessage(message);
  EXPECT_THROW(client_connection->ReceiveMessage(), Connection::BadAcknowledgeException);
}
