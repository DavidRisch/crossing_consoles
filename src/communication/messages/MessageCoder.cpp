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

std::vector<uint8_t> MessageCoder::Encode(Message *message) {
  std::vector<uint8_t> output;
  output.reserve(sizeof(ProtocolDefinition::start_sequence) + 1);

  auto number_start_sequence = ProtocolDefinition::GetNumericStartSequence();
  WriteToStream(output, number_start_sequence, sizeof(ProtocolDefinition::start_sequence));
  output.push_back(static_cast<uint8_t>(message->GetMessageType()));

  // TODO: CRC implementation
  crc_t crc = 0xEFEFEFEFu;
  WriteToStream(output, crc, crc_length);

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

  return output;
}

std::unique_ptr<Message> MessageCoder::Decode(IInputStream &stream) {
  size_t current_position = 0;

  auto found_start_sequence =
      ReadFromStream<ProtocolDefinition::start_sequence_t>(stream, sizeof(ProtocolDefinition::start_sequence));
  assert(found_start_sequence == ProtocolDefinition::GetNumericStartSequence());

  auto message_type_value = ReadFromStream<char>(stream, sizeof(char));
  assert(message_type_value >= 0);
  assert(message_type_value <= static_cast<uint8_t>(MessageType::HIGHEST_ELEMENT));
  auto message_type = static_cast<MessageType>(message_type_value);

  auto crc = ReadFromStream<crc_t>(stream, crc_length);
  // TODO: CRC implementation (throw CrcIncorrectException)

  ProtocolDefinition::payload_length_t payload_length = 0;
  if (message_type == MessageType::PAYLOAD) {
    payload_length =
        ReadFromStream<ProtocolDefinition::payload_length_t>(stream, sizeof(ProtocolDefinition::payload_length_t));
  }

  // TODO: create real metadata
  MessageMetaData message_meta_data(123, 456);

  switch (message_type) {
    case MessageType::KEEP_ALIVE:
      return std::make_unique<KeepAliveMessage>(KeepAliveMessage(0, message_meta_data));
    case MessageType::CONNECTION_REQUEST:
      return std::make_unique<ConnectionRequestMessage>(ConnectionRequestMessage(0, message_meta_data));
    case MessageType::CONNECTION_RESPONSE:
      return std::make_unique<ConnectionResponseMessage>(ConnectionResponseMessage(0, message_meta_data));
    case MessageType::PAYLOAD:
      std::vector<uint8_t> payload;
      payload.resize(payload_length);
      size_t read_length = 0;
      uint8_t *read_pointer = &payload[0];
      while (read_length < payload_length) {
        read_length += stream.Read(read_pointer, payload_length - read_length);
        read_pointer += read_length;
      }

      return std::make_unique<PayloadMessage>(PayloadMessage(0, payload, message_meta_data));
  }
}
