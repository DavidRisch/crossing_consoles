#include "AcknowledgeMessage.h"

using namespace communication;
using namespace communication::message_layer;
AcknowledgeMessage::AcknowledgeMessage(ProtocolDefinition::sequence_t acknowledged_msg_sequence)
    : Message()
    , acknowledged_msg_sequence(acknowledged_msg_sequence) {
}

AcknowledgeMessage::AcknowledgeMessage(ProtocolDefinition::sequence_t acknowledged_msg_sequence,
                                       ProtocolDefinition::sequence_t sequence)
    : Message(sequence)
    , acknowledged_msg_sequence(acknowledged_msg_sequence) {
}

MessageType AcknowledgeMessage::message_type = MessageType::ACKNOWLEDGE;

MessageType AcknowledgeMessage::GetMessageType() const {
  return message_type;
}
ProtocolDefinition::sequence_t AcknowledgeMessage::GetAcknowledgedMessageSequence() const {
  return acknowledged_msg_sequence;
}
