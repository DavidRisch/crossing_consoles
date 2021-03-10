#include "KeepAliveMessage.h"

using namespace communication;
using namespace communication::message_layer;

KeepAliveMessage::KeepAliveMessage(ProtocolDefinition::sequence_t sequence)
    : Message(sequence) {
}
KeepAliveMessage::KeepAliveMessage()
    : Message() {
}

MessageType KeepAliveMessage::message_type = MessageType::KEEP_ALIVE;

MessageType KeepAliveMessage::GetMessageType() const {
  return message_type;
}
