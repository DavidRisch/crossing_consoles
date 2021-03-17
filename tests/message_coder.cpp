#include <gtest/gtest.h>

#include "../src/communication/ProtocolDefinition.h"
#include "../src/communication/byte_layer/byte_stream/MockInputStream.h"
#include "../src/communication/message_layer/message/AcknowledgeMessage.h"
#include "../src/communication/message_layer/message/KeepAliveMessage.h"
#include "../src/communication/message_layer/message/PayloadMessage.h"
#include "../src/communication/message_layer/message_stream/MessageCoder.h"

using namespace communication;
using namespace communication::byte_layer;
using namespace communication;
using namespace communication::message_layer;

TEST(MessageCoder, KeepAliveMessage) {
  KeepAliveMessage original_message = KeepAliveMessage();

  auto encoded_message = MessageCoder::Encode(&original_message);

  MockInputStream mock_input_stream;
  mock_input_stream.AddData(encoded_message);

  auto decoded_message = MessageCoder::Decode(mock_input_stream);
  EXPECT_TRUE(mock_input_stream.IsEmpty());

  EXPECT_EQ(original_message.GetMessageType(), decoded_message->GetMessageType());
}

TEST(MessageCoder, PayloadMessage) {
  std::vector<uint8_t> original_payload;

  int original_payload_length = 123;
  original_payload.reserve(original_payload_length);
  for (int i = 0; i < original_payload_length; ++i) {
    original_payload.push_back(i);
  }
  PayloadMessage original_message(original_payload);

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

TEST(MessageCoder, AcknowledgeMessage) {
  ProtocolDefinition::sequence_t sequence = 70;
  AcknowledgeMessage original_message(sequence);

  auto encoded_message = MessageCoder::Encode(&original_message);

  MockInputStream mock_input_stream;
  mock_input_stream.AddData(encoded_message);

  auto decoded_message = MessageCoder::Decode(mock_input_stream);
  EXPECT_TRUE(mock_input_stream.IsEmpty());

  EXPECT_EQ(original_message.GetMessageType(), decoded_message->GetMessageType());
  auto decoded_message_ack = dynamic_cast<AcknowledgeMessage &>(*decoded_message);
  EXPECT_EQ(original_message.GetAcknowledgedMessageSequence(), decoded_message_ack.GetAcknowledgedMessageSequence());
}

TEST(MessageCoder, CrcIncorrectException) {
  std::vector<uint8_t> original_payload;

  int original_payload_length = 185;
  original_payload.reserve(original_payload_length);
  for (int i = 0; i < original_payload_length; ++i) {
    original_payload.push_back(i % 20);
  }
  PayloadMessage original_message(original_payload);
  auto encoded_message = MessageCoder::Encode(&original_message);

  // only payload is manipulated, other cases are tested separately
  size_t payload_offset = sizeof(ProtocolDefinition::flag) + sizeof(MessageType) +
                          sizeof(ProtocolDefinition::payload_length_t) + sizeof(ProtocolDefinition::sequence_t);

  for (size_t i = payload_offset; i < encoded_message.size() - sizeof(ProtocolDefinition::flag); ++i) {
    std::vector<uint8_t> bad_encoded_message(encoded_message);
    bad_encoded_message.at(i) ^= 1;  // change a single byte of the encoded message

    MockInputStream mock_input_stream;
    mock_input_stream.AddData(bad_encoded_message);
    EXPECT_THROW(MessageCoder::Decode(mock_input_stream), MessageCoder::CrcIncorrectException);
  }
}

TEST(MessageCoder, InvalidMessageExceptionNoEndFlag) {
  // test for missing end flag
  std::vector<uint8_t> original_payload;

  int original_payload_length = 185;
  original_payload.reserve(original_payload_length);
  for (int i = 0; i < original_payload_length; ++i) {
    original_payload.push_back(i);
  }
  PayloadMessage original_message(original_payload);
  auto encoded_message = MessageCoder::Encode(&original_message);

  std::vector<uint8_t> bad_encoded_message(encoded_message);
  bad_encoded_message.back() ^= 1;

  MockInputStream mock_input_stream;
  mock_input_stream.AddData(bad_encoded_message);
  EXPECT_THROW(MessageCoder::Decode(mock_input_stream), MessageCoder::InvalidMessageException);
}

class InvalidMessageCoder : public ::testing::Test {
 public:
  static void ReplaceWithSequence(uint8_t sequence, size_t offset = 0) {
    std::vector<uint8_t> original_payload;

    int original_payload_length = 185;
    original_payload.reserve(original_payload_length);
    for (int i = 0; i < original_payload_length; ++i) {
      original_payload.push_back(i);
    }
    PayloadMessage original_message(original_payload);
    auto encoded_message = MessageCoder::Encode(&original_message);

    for (size_t i = sizeof(sequence); i < encoded_message.size() - offset; ++i) {
      if (encoded_message[i] == sequence) {
        // do not replace if byte equals the desired sequence
        continue;
      }
      size_t number_of_preceding_escapes;
      for (number_of_preceding_escapes = 0; number_of_preceding_escapes < i; number_of_preceding_escapes++) {
        if (encoded_message.at(i - number_of_preceding_escapes - 1) != ProtocolDefinition::escape) {
          break;
        }
      }

      // odd number of preceding escapes does not lead to Exception, sequence would remain properly escaped
      if (number_of_preceding_escapes % 2 == 0) {
        std::vector<uint8_t> bad_encoded_message(encoded_message);
        bad_encoded_message.at(i) = sequence;  // change to sequence
        MockInputStream mock_input_stream;
        mock_input_stream.AddData(bad_encoded_message);
        EXPECT_THROW(MessageCoder::Decode(mock_input_stream), MessageCoder::InvalidMessageException);
      }
    }
  }
};

TEST_F(InvalidMessageCoder, ExceptionEarlyFlag) {
  // test for flags in msg that are not properly escaped
  ReplaceWithSequence(ProtocolDefinition::flag);
}

TEST_F(InvalidMessageCoder, ExceptionEscape) {
  // test for escape sequences that are not properly escaped
  // exclude CRC Value and last flag (tested separately)
  size_t offset = sizeof(ProtocolDefinition::flag) + sizeof(crc_value_t);
  ReplaceWithSequence(ProtocolDefinition::escape, offset);
}

TEST_F(InvalidMessageCoder, MessageSequenceisFlag) {
  // test value of flag as msg sequence - needs to be properly escaped
  ProtocolDefinition::sequence_t sequence = ProtocolDefinition::flag;
  KeepAliveMessage original_message(sequence);

  auto encoded_message = MessageCoder::Encode(&original_message);

  MockInputStream mock_input_stream;
  mock_input_stream.AddData(encoded_message);

  auto decoded_message = MessageCoder::Decode(mock_input_stream);
  EXPECT_EQ(decoded_message->GetMessageSequence(), sequence);
}

TEST_F(InvalidMessageCoder, MessageSequenceisEscape) {
  // test value of escape as msg sequence - needs to be properly escaped
  ProtocolDefinition::sequence_t sequence = ProtocolDefinition::escape;
  KeepAliveMessage original_message(sequence);

  auto encoded_message = MessageCoder::Encode(&original_message);

  MockInputStream mock_input_stream;
  mock_input_stream.AddData(encoded_message);

  auto decoded_message = MessageCoder::Decode(mock_input_stream);
  EXPECT_EQ(decoded_message->GetMessageSequence(), sequence);
}
