#include "ConnectionRequestMessage.h"

using namespace communication;
using namespace communication::message_layer;

ConnectionRequestMessage::ConnectionRequestMessage(ProtocolDefinition::sequence_t sequence)
    : Message(sequence) {
}
ConnectionRequestMessage::ConnectionRequestMessage()
    : Message() {
}

MessageType ConnectionRequestMessage::message_type = MessageType::CONNECTION_REQUEST;

MessageType ConnectionRequestMessage::GetMessageType() const {
  return message_type;
}
