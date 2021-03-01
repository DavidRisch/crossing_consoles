

#include "../src/communication/connection_layer/Connection.h"

#include <gtest/gtest.h>

#include <thread>

#include "../src/communication/low_level/ByteServer.h"
#include "../src/communication/low_level/MockBidirectionalByteStream.h"
#include "../src/communication/messages/KeepAliveMessage.h"
#include "../src/communication/messages/PayloadMessage.h"

TEST(Connection, Simple) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();
  auto &server_side = *stream_pair.first;
  auto &client_side = *stream_pair.second;

  auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
  auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

  auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
  auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

  std::shared_ptr<Connection> server_connection;

  std::thread server_thread([&server_connection, &server_message_input_stream, &server_message_output_stream] {
    server_connection = Connection::CreateServerSide(server_message_input_stream, server_message_output_stream);
  });

  auto client_connection = Connection::CreateClientSide(client_message_input_stream, client_message_output_stream);
  server_thread.join();

  address_t target_address = 1234;
  {
    KeepAliveMessage original_message(target_address);

    client_connection->SendMessage(&original_message);

    auto received_message = server_connection->ReceiveMessage();
    EXPECT_EQ(original_message.GetMessageType(), received_message->GetMessageType());
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

// TODO: test timeout

// TODO: test failed connection
