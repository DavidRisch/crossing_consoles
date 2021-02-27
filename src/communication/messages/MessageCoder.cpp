#include "MessageCoder.h"

#include <cassert>
#include <memory>
#include <optional>

#include "ConnectionRequestMessage.h"
#include "ConnectionResponseMessage.h"
#include "KeepAliveMessage.h"
#include "PayloadMessage.h"

template <typename T>
void WriteToVector(std::vector<uint8_t> &output, T input, unsigned int input_length) {
  for (unsigned int i = 0; i < input_length; ++i) {
    unsigned int byte_position = input_length - 1 - i;  // to get big endian mode
    uint8_t val = *(reinterpret_cast<uint8_t *>(&input) + byte_position);
    output.push_back(val);
  }
}

template <typename T>
T ReadFromBuffer(const uint8_t *receive_buffer, size_t &current_position, size_t max_length, unsigned int type_length) {
  T output = 0;
  for (unsigned int i = 0; i < type_length; ++i) {
    unsigned int byte_position = type_length - 1 - i;  // to get big endian mode

    if (current_position >= max_length) {
      throw MessageCoder::InputTooShortException();
    }

    T byte = receive_buffer[current_position++];
    output |= byte << 8 * byte_position;
  }
  return output;
}

std::vector<uint8_t> MessageCoder::Encode(Message *message) {
  std::vector<uint8_t> output;
  output.reserve(start_sequence_length + 1);

  WriteToVector(output, start_sequence, start_sequence_length);
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
      payload_length_t payload_length = payload.size();

      WriteToVector(output, payload_length, sizeof(payload_length));

      for (const auto &payload_byte : payload) {
        output.push_back(payload_byte);
      }
      break;
  }

  crc_value_t crc = CRCHandler::CalculateCRCValue(output.data(), output.size());
  WriteToVector(output, crc, crc_length);

  return output;
}

std::unique_ptr<Message> MessageCoder::Decode(const uint8_t *receive_buffer, size_t receive_buffer_length) {
  size_t current_position = 0;

  auto found_start_sequence =
      ReadFromBuffer<start_sequence_t>(receive_buffer, current_position, receive_buffer_length, start_sequence_length);
  assert(found_start_sequence == start_sequence);

  auto message_type_value = ReadFromBuffer<char>(receive_buffer, current_position, receive_buffer_length, sizeof(char));
  assert(message_type_value >= 0);
  assert(message_type_value <= static_cast<uint8_t>(MessageType::HIGHEST_ELEMENT));
  auto message_type = static_cast<MessageType>(message_type_value);

  payload_length_t payload_length = 0;
  if (message_type == MessageType::PAYLOAD) {
    payload_length = ReadFromBuffer<payload_length_t>(receive_buffer, current_position, receive_buffer_length,
                                                      sizeof(payload_length_t));
  }

  size_t crc_position = current_position + payload_length;
  auto crc = ReadFromBuffer<uint32_t>(receive_buffer, crc_position, receive_buffer_length, crc_length);

  if (!CRCHandler::CheckCRCValue(receive_buffer, crc_position - crc_length, crc)) {
    throw CrcIncorrectException();
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
      payload.reserve(payload_length);

      for (int i = 0; i < payload_length; ++i) {
        payload.push_back(receive_buffer[current_position++]);
      }

      return std::make_unique<PayloadMessage>(PayloadMessage(0, payload, message_meta_data));
  }
}
