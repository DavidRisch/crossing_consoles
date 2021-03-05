#include "PayloadMessage.h"

#include <utility>

using namespace communication;
using namespace communication::message_layer;

PayloadMessage::PayloadMessage(address_t address, std::vector<uint8_t> payload, MessageMetaData meta_data,
                               ProtocolDefinition::sequence_t sequence)
    : Message(address, sequence, meta_data)
    , payload(std::move(payload)) {
}
PayloadMessage::PayloadMessage(address_t address, std::vector<uint8_t> payload, ProtocolDefinition::sequence_t sequence)
    : Message(address, sequence)
    , payload(std::move(payload)) {
}

MessageType PayloadMessage::message_type = MessageType::PAYLOAD;

MessageType PayloadMessage::GetMessageType() const {
  return message_type;
}

std::vector<uint8_t> PayloadMessage::GetPayload() const {
  return payload;
}
