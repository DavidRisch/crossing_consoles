#include "MessageCoder.h"

#include <cassert>
#include <memory>
#include <optional>

#include "../ProtocolDefinition.h"
#include "ConnectionRequestMessage.h"
#include "ConnectionResponseMessage.h"
#include "KeepAliveMessage.h"
#include "PayloadMessage.h"

template <typename T>
void WriteToStream(std::vector<uint8_t> &output, T input, unsigned int input_length) {
  for (unsigned int i = 0; i < input_length; ++i) {
    unsigned int byte_position = input_length - 1 - i;  // to get big endian mode
    uint8_t val = *(reinterpret_cast<uint8_t *>(&input) + byte_position);
    output.push_back(val);
  }
}

template <typename T>
T ReadFromStream(IInputStream &stream, unsigned int type_length) {
  T output = 0;
  for (unsigned int i = 0; i < type_length; ++i) {
    unsigned int byte_position = type_length - 1 - i;  // to get big endian mode

    T byte = 0;
    stream.Read(reinterpret_cast<uint8_t *>(&byte), 1);
    output |= byte << 8 * byte_position;
  }
  return output;
}

template <typename T>
T ReadFromStreamWithCopy(IInputStream &stream, unsigned int type_length, std::vector<uint8_t> &raw_message) {
  T output = 0;
  for (unsigned int i = 0; i < type_length; ++i) {
    unsigned int byte_position = type_length - 1 - i;  // to get big endian mode

    T byte = 0;
    stream.Read(reinterpret_cast<uint8_t *>(&byte), 1);
    raw_message.push_back(byte);
    output |= byte << 8 * byte_position;
  }
  return output;
}

std::vector<uint8_t> MessageCoder::Encode(Message *message) {
  std::vector<uint8_t> output;
  output.reserve(sizeof(ProtocolDefinition::start_sequence) + 1);

  auto number_start_sequence = ProtocolDefinition::GetNumericStartSequence();
  WriteToStream(output, number_start_sequence, sizeof(ProtocolDefinition::start_sequence));
  output.push_back(static_cast<uint8_t>(message->GetMessageType()));

  switch (message->GetMessageType()) {
    case MessageType::KEEP_ALIVE:
    case MessageType::CONNECTION_REQUEST:
    case MessageType::CONNECTION_RESPONSE:
      // no payload necessary
      break;
    case MessageType::PAYLOAD:
      auto *payload_message = reinterpret_cast<PayloadMessage *>(message);
      const auto &payload = payload_message->GetPayload();
      ProtocolDefinition::payload_length_t payload_length = payload.size();

      WriteToStream(output, payload_length, sizeof(payload_length));

      for (const auto &payload_byte : payload) {
        output.push_back(payload_byte);
      }
      break;
  }

  int crc_start_offset = sizeof(ProtocolDefinition::start_sequence);
  crc_value_t crc = CRCHandler::CalculateCRCValue(output.data() + crc_start_offset, output.size() - crc_start_offset);
  WriteToStream(output, crc, crc_length);

  return output;
}

std::shared_ptr<Message> MessageCoder::Decode(IInputStream &stream, bool expect_start_sequence) {
  if (expect_start_sequence) {
    auto found_start_sequence =
        ReadFromStream<ProtocolDefinition::start_sequence_t>(stream, sizeof(ProtocolDefinition::start_sequence));
    assert(found_start_sequence == ProtocolDefinition::GetNumericStartSequence());
  }

  // used to generate crc
  std::vector<uint8_t> raw_message;

  auto message_type_value = ReadFromStreamWithCopy<char>(stream, sizeof(char), raw_message);
  assert(message_type_value >= 0);
  assert(message_type_value <= static_cast<uint8_t>(MessageType::HIGHEST_ELEMENT));
  auto message_type = static_cast<MessageType>(message_type_value);

  ProtocolDefinition::payload_length_t payload_length = 0;
  if (message_type == MessageType::PAYLOAD) {
    payload_length = ReadFromStreamWithCopy<ProtocolDefinition::payload_length_t>(
        stream, sizeof(ProtocolDefinition::payload_length_t), raw_message);
  }

  // TODO: create real metadata
  MessageMetaData message_meta_data(123, 456);

  std::shared_ptr<Message> message;

  switch (message_type) {
    case MessageType::KEEP_ALIVE:
      // TODO: use real address (also applies to lines below
      message = std::make_shared<KeepAliveMessage>(0, message_meta_data);
      break;
    case MessageType::CONNECTION_REQUEST:
      message = std::make_shared<ConnectionRequestMessage>(0, message_meta_data);
      break;
    case MessageType::CONNECTION_RESPONSE:
      message = std::make_shared<ConnectionResponseMessage>(0, message_meta_data);
      break;
    case MessageType::PAYLOAD: {
      std::vector<uint8_t> payload;
      payload.resize(payload_length);
      size_t read_length = 0;
      uint8_t *read_pointer = &payload[0];
      while (read_length < payload_length) {
        read_length += stream.Read(read_pointer, payload_length - read_length);
        read_pointer += read_length;
      }

      raw_message.insert(raw_message.end(), payload.begin(), payload.end());

      message = std::make_shared<PayloadMessage>(0, payload, message_meta_data);
      break;
    }
    default:
      assert(false);
  }

  auto read_crc = ReadFromStream<crc_value_t>(stream, crc_length);

  if (!CRCHandler::CheckCRCValue(raw_message.data(), raw_message.size(), read_crc)) {
    throw CrcIncorrectException();
  }

  return message;
}
