#include <gtest/gtest.h>

#include "../src/communication/low_level/ByteServer.h"
#include "../src/communication/low_level/MockBidirectionalByteStream.h"
#include "../src/communication/message_layer/MessageInputStream.h"
#include "../src/communication/message_layer/MessageOutputStream.h"
#include "../src/communication/messages/KeepAliveMessage.h"

TEST(MessageStream, Simple) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();

  MessageInputStream message_input_stream(*stream_pair.first);
  MessageOutputStream message_output_stream(*stream_pair.second);

  address_t target_address = 1234;
  KeepAliveMessage original_message(target_address);

  message_output_stream.SendMessage(&original_message);

  auto received_message = message_input_stream.ReceiveMessage();

  EXPECT_EQ(original_message.GetMessageType(), received_message->GetMessageType());
}

TEST(MessageStream, WithPadding) {
  auto stream_pair = MockBidirectionalByteStream::CreatePair();

  MessageInputStream message_input_stream(*stream_pair.first);
  MessageOutputStream message_output_stream(*stream_pair.second);

  address_t target_address = 1234;
  KeepAliveMessage original_message(target_address);

  u_int8_t padding[10] = {};
  stream_pair.second->Send(padding, sizeof(padding));
  message_output_stream.SendMessage(&original_message);
  stream_pair.second->Send(padding, sizeof(padding));

  auto received_message = message_input_stream.ReceiveMessage();

  EXPECT_EQ(original_message.GetMessageType(), received_message->GetMessageType());
}
