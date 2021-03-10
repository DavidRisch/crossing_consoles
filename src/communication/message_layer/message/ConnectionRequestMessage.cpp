#include "ConnectionRequestMessage.h"

using namespace communication;
using namespace communication::message_layer;

ConnectionRequestMessage::ConnectionRequestMessage(ProtocolDefinition::address_t address,
                                                   ProtocolDefinition::sequence_t sequence)
    : Message(address, sequence) {
}
ConnectionRequestMessage::ConnectionRequestMessage(ProtocolDefinition::address_t address)
    : Message(address) {
}

MessageType ConnectionRequestMessage::message_type = MessageType::CONNECTION_REQUEST;

MessageType ConnectionRequestMessage::GetMessageType() const {
  return message_type;
}
