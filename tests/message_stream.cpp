#include <gtest/gtest.h>

#include <thread>

#include "../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../src/communication/message_layer/message/KeepAliveMessage.h"
#include "../src/communication/message_layer/message/PayloadMessage.h"
#include "../src/communication/message_layer/message_stream/MessageInputStream.h"
#include "../src/communication/message_layer/message_stream/MessageOutputStream.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication;
using namespace communication::message_layer;

TEST(MessageStream, NoBlocking) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();

  MessageInputStream message_input_stream(stream_pair.first);

  auto received_message = message_input_stream.ReceiveMessage(false);

  // assert that no message was received
  EXPECT_FALSE(received_message);
}

TEST(MessageStream, Simple) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();

  MessageInputStream message_input_stream(stream_pair.first);
  MessageOutputStream message_output_stream(stream_pair.second);

  ProtocolDefinition::address_t target_address = 1234;
  KeepAliveMessage original_message(target_address);

  message_output_stream.SendMessage(&original_message);

  auto received_message = message_input_stream.ReceiveMessage();

  EXPECT_EQ(original_message.GetMessageType(), received_message->GetMessageType());
}

TEST(MessageStream, WithPadding) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();

  MessageInputStream message_input_stream(stream_pair.first);
  MessageOutputStream message_output_stream(stream_pair.second);

  ProtocolDefinition::address_t target_address = 1234;
  KeepAliveMessage original_message(target_address);

  uint8_t padding[10] = {};
  stream_pair.second->Send(padding, sizeof(padding));
  message_output_stream.SendMessage(&original_message);
  stream_pair.second->Send(padding, sizeof(padding));

  auto received_message = message_input_stream.ReceiveMessage();

  EXPECT_EQ(original_message.GetMessageType(), received_message->GetMessageType());
}

TEST(MessageStream, Multiple) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();

  MessageInputStream message_input_stream(stream_pair.first);
  MessageOutputStream message_output_stream(stream_pair.second);

  ProtocolDefinition::address_t target_address = 1234;
  for (int i = 0; i < 10; ++i) {
    std::vector<uint8_t> payload;
    payload.push_back(i);
    PayloadMessage original_message(target_address, payload);

    message_output_stream.SendMessage(&original_message);

    auto received_message = message_input_stream.ReceiveMessage();

    EXPECT_EQ(original_message.GetMessageType(), received_message->GetMessageType());
    auto &payload_message = dynamic_cast<PayloadMessage &>(*received_message);

    EXPECT_EQ(payload_message.GetPayload().size(), original_message.GetPayload().size());
    EXPECT_EQ(payload_message.GetPayload().at(0), original_message.GetPayload().at(0));
  }
}

TEST(MessageStream, Threaded) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();

  auto a_message_input_stream = std::make_shared<MessageInputStream>(stream_pair.second);
  auto b_message_output_stream = std::make_shared<MessageOutputStream>(stream_pair.first);

  auto b_message_input_stream = std::make_shared<MessageInputStream>(stream_pair.first);
  auto a_message_output_stream = std::make_shared<MessageOutputStream>(stream_pair.second);

  ProtocolDefinition::address_t target_address = 1234;

  int test_message_count = 100;

  std::thread a_thread([&a_message_input_stream, &a_message_output_stream, target_address, test_message_count] {
    for (int i = 0; i < test_message_count; ++i) {
      std::vector<uint8_t> payload;
      payload.push_back(i);
      PayloadMessage original_message(target_address, payload);

      a_message_output_stream->SendMessage(&original_message);
    }
    for (int i = 0; i < test_message_count; ++i) {
      auto received_message = a_message_input_stream->ReceiveMessage();

      EXPECT_EQ(received_message->GetMessageType(), MessageType::PAYLOAD);
      auto &payload_message = dynamic_cast<PayloadMessage &>(*received_message);

      EXPECT_EQ(payload_message.GetPayload().size(), 1);
      EXPECT_EQ(payload_message.GetPayload().at(0), i + test_message_count);
    }
  });

  for (int i = 0; i < test_message_count; ++i) {
    auto received_message = b_message_input_stream->ReceiveMessage();

    EXPECT_EQ(received_message->GetMessageType(), MessageType::PAYLOAD);
    auto &payload_message = dynamic_cast<PayloadMessage &>(*received_message);

    EXPECT_EQ(payload_message.GetPayload().size(), 1);
    EXPECT_EQ(payload_message.GetPayload().at(0), i);
  }
  for (int i = 0; i < test_message_count; ++i) {
    std::vector<uint8_t> payload;
    payload.push_back(i + test_message_count);
    PayloadMessage original_message(target_address, payload);

    b_message_output_stream->SendMessage(&original_message);
  }

  a_thread.join();
}
