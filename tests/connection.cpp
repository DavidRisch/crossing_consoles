#include "../src/communication/connection_layer/connection/Connection.h"

#include <gtest/gtest.h>

#include "fixtures/Connections.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication::connection_layer;
using namespace communication::message_layer;

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

TEST_F(Connections, ClientClosesConnection) {
  // Client sends ConnectionReset message

  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = stream_pair.first;
  auto &client_side = stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  make_connections(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                   server_message_output_stream);
  close_connection(client_connection, server_connection);
}

TEST_F(Connections, ServerClosesConnection) {
  // Server sends ConnectionReset message

  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = stream_pair.first;
  auto &client_side = stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  make_connections(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                   server_message_output_stream);
  close_connection(server_connection, client_connection);
}

TEST_F(Connections, NoConnectionReset) {
  // any other message beside ConnectionReset should not trigger ready_to_close_flag
  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = stream_pair.first;
  auto &client_side = stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  make_connections(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                   server_message_output_stream);

  for (auto &message : test_messages_all_types) {
    if (message->GetMessageType() == MessageType::CONNECTION_RESET) {
      continue;
    }

    client_connection->SendMessage(message);

    if (message->GetMessageType() == MessageType::ACKNOWLEDGE) {
      // server throws exception if acknowledge is received in state ready
      EXPECT_THROW(server_connection->ReceiveMessage(), Connection::BadAcknowledgeException);
      continue;
    }

    if (message->GetMessageType() == MessageType::NOT_ACKNOWLEDGE) {
      // server resends last sent message
      server_connection->ReceiveMessage();
      auto received_message_client = client_connection->ReceiveMessage();
      auto received_message_server = server_connection->ReceiveMessage();
      ASSERT_EQ(received_message_server, nullptr);
      ASSERT_EQ(received_message_client, nullptr);

      ASSERT_FALSE(client_connection->ConnectionClosed());
      continue;
    }

    auto received_message_server = server_connection->ReceiveMessage();
    ASSERT_NE(received_message_server, nullptr);

    auto received_message_client = client_connection->ReceiveMessage();
    ASSERT_NE(received_message_client, nullptr);
    ASSERT_EQ(received_message_client->GetMessageType(), message_layer::MessageType::ACKNOWLEDGE);

    // Connection not ready to be closed, no Connection Reset was sent or acknowledged
    ASSERT_FALSE(client_connection->ConnectionClosed());
  }
}
