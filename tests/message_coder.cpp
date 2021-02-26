#include <gtest/gtest.h>

#include "../src/communication/messages/KeepAliveMessage.h"
#include "../src/communication/messages/MessageCoder.h"
#include "../src/communication/messages/PayloadMessage.h"

TEST(MessageCoder, KeepAliveMessage) {
  address_t target_address = 1234;
  KeepAliveMessage original_message(target_address);

  auto encoded_message = MessageCoder::Encode(&original_message);

  auto decoded_message = MessageCoder::Decode(encoded_message.data(), encoded_message.size());

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

  auto decoded_message = MessageCoder::Decode(encoded_message.data(), encoded_message.size());

  EXPECT_EQ(original_message.GetMessageType(), decoded_message->GetMessageType());
  auto &pm = dynamic_cast<PayloadMessage &>(*decoded_message);
  const auto &decoded_payload = pm.GetPayload();

  ASSERT_EQ(original_payload_length, pm.GetPayload().size());

  for (int i = 0; i < original_payload_length; ++i) {
    EXPECT_EQ(original_payload.at(i), decoded_payload.at(i));
  }
}

TEST(MessageCoder, InputTooShortException) {
  address_t target_address = 1234;
  std::vector<uint8_t> original_payload;

  int original_payload_length = 123;
  original_payload.reserve(original_payload_length);
  for (int i = 0; i < original_payload_length; ++i) {
    original_payload.push_back(i);
  }
  PayloadMessage original_message(target_address, original_payload);

  auto encoded_message = MessageCoder::Encode(&original_message);

  for (size_t i = 0; i < encoded_message.size(); ++i) {
    EXPECT_THROW(MessageCoder::Decode(encoded_message.data(), i), MessageCoder::InputTooShortException);
  }
}

// TODO: CRC implementation (add CRC test cases)
