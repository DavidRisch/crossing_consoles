#include "ConnectionResponseMessage.h"

using namespace communication;
using namespace communication::message_layer;

ConnectionResponseMessage::ConnectionResponseMessage(ProtocolDefinition::address_t address,
                                                     ProtocolDefinition::sequence_t sequence)
    : Message(address, sequence) {
}
ConnectionResponseMessage::ConnectionResponseMessage(ProtocolDefinition::address_t address)
    : Message(address) {
}

MessageType ConnectionResponseMessage::message_type = MessageType::CONNECTION_RESPONSE;

MessageType ConnectionResponseMessage::GetMessageType() const {
  return message_type;
}
