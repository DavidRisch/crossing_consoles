#include "PayloadMessage.h"

#include <utility>

PayloadMessage::PayloadMessage(address_t address, std::vector<uint8_t> payload, MessageMetaData meta_data)
    : payload(std::move(payload))
    , Message(address, message_type, meta_data) {
}
PayloadMessage::PayloadMessage(address_t address, std::vector<uint8_t> payload)
    : payload(std::move(payload))
    , Message(address, message_type) {
}

MessageType PayloadMessage::message_type = PAYLOAD;

std::vector<uint8_t> PayloadMessage::GetPayload() const {
  return std::vector<uint8_t>();
}
