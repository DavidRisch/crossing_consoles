#include <gtest/gtest.h>

#include "../src/communication/ProtocolDefinition.h"
#include "../src/communication/byte_layer/byte_stream/MockInputStream.h"
#include "../src/communication/message_layer/message/KeepAliveMessage.h"
#include "../src/communication/message_layer/message/PayloadMessage.h"
#include "../src/communication/message_layer/message_stream/MessageCoder.h"

TEST(MessageCoder, KeepAliveMessage) {
  address_t target_address = 1234;
  KeepAliveMessage original_message(target_address);

  auto encoded_message = MessageCoder::Encode(&original_message);

  MockInputStream mock_input_stream;
  mock_input_stream.AddData(encoded_message);

  auto decoded_message = MessageCoder::Decode(mock_input_stream);
  EXPECT_TRUE(mock_input_stream.IsEmpty());

  EXPECT_EQ(original_message.GetMessageType(), decoded_message->GetMessageType());
}

TEST(MessageCoder, PayloadMessage) {
  address_t target_address = 1234;
  std::vector<uint8_t> original_payload;

  int original_payload_length = 123;
  original_payload.reserve(original_payload_length);
  for (int i = 0; i < original_payload_length; ++i) {
    original_payload.push_back(i);
  }
  PayloadMessage original_message(target_address, original_payload);

  auto encoded_message = MessageCoder::Encode(&original_message);

  MockInputStream mock_input_stream;
  mock_input_stream.AddData(encoded_message);
  auto decoded_message = MessageCoder::Decode(mock_input_stream);
  EXPECT_TRUE(mock_input_stream.IsEmpty());

  EXPECT_EQ(original_message.GetMessageType(), decoded_message->GetMessageType());
  auto &pm = dynamic_cast<PayloadMessage &>(*decoded_message);
  const auto &decoded_payload = pm.GetPayload();

  ASSERT_EQ(original_payload_length, pm.GetPayload().size());

  for (int i = 0; i < original_payload_length; ++i) {
    EXPECT_EQ(original_payload.at(i), decoded_payload.at(i));
  }
}

TEST(MessageCoder, CrcIncorrectException) {
  // Create a message
  address_t target_address = 1234;
  std::vector<uint8_t> original_payload;

  int original_payload_length = 185;
  original_payload.reserve(original_payload_length);
  for (int i = 0; i < original_payload_length; ++i) {
    original_payload.push_back(i);
  }
  PayloadMessage original_message(target_address, original_payload);
  auto encoded_message = MessageCoder::Encode(&original_message);

  for (size_t i = sizeof(ProtocolDefinition::start_sequence); i < encoded_message.size(); ++i) {
    std::vector<uint8_t> bad_encoded_message(encoded_message);
    bad_encoded_message.at(i) ^= 1;  // change a single byte of the encoded message

    MockInputStream mock_input_stream;
    mock_input_stream.AddData(bad_encoded_message);
    EXPECT_THROW(MessageCoder::Decode(mock_input_stream), MessageCoder::CrcIncorrectException);
  }
}
