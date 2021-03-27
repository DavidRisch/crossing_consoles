#include "NotAcknowledgeMessage.h"

using namespace communication;
using namespace communication::message_layer;

NotAcknowledgeMessage::NotAcknowledgeMessage(ProtocolDefinition::sequence_t sequence)
    : Message(sequence) {
}
NotAcknowledgeMessage::NotAcknowledgeMessage()
    : Message() {
}

MessageType NotAcknowledgeMessage::message_type = MessageType::NOT_ACKNOWLEDGE;

MessageType NotAcknowledgeMessage::GetMessageType() const {
  return message_type;
}
