#include "KeepAliveMessage.h"

using namespace communication;
using namespace communication::message_layer;

KeepAliveMessage::KeepAliveMessage(ProtocolDefinition::address_t address, ProtocolDefinition::sequence_t sequence)
    : Message(address, sequence) {
}
KeepAliveMessage::KeepAliveMessage(ProtocolDefinition::address_t address)
    : Message(address) {
}

MessageType KeepAliveMessage::message_type = MessageType::KEEP_ALIVE;

MessageType KeepAliveMessage::GetMessageType() const {
  return message_type;
}
