#include "ConnectionResponseMessage.h"

using namespace communication;
using namespace communication::message_layer;

ConnectionResponseMessage::ConnectionResponseMessage(address_t address, MessageMetaData meta_data,
                                                     ProtocolDefinition::sequence_t sequence)
    : Message(address, sequence, meta_data) {
}
ConnectionResponseMessage::ConnectionResponseMessage(address_t address, ProtocolDefinition::sequence_t sequence)
    : Message(address, sequence) {
}

MessageType ConnectionResponseMessage::message_type = MessageType::CONNECTION_RESPONSE;

MessageType ConnectionResponseMessage::GetMessageType() const {
  return message_type;
}
