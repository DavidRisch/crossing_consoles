#include "ConnectionResponseMessage.h"

using namespace communication;
using namespace communication::message_layer;

ConnectionResponseMessage::ConnectionResponseMessage(ProtocolDefinition::sequence_t sequence)
    : Message(sequence) {
}
ConnectionResponseMessage::ConnectionResponseMessage()
    : Message() {
}

MessageType ConnectionResponseMessage::message_type = MessageType::CONNECTION_RESPONSE;

MessageType ConnectionResponseMessage::GetMessageType() const {
  return message_type;
}
