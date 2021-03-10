#include "AcknowledgeMessage.h"

using namespace communication;
using namespace communication::message_layer;
AcknowledgeMessage::AcknowledgeMessage(ProtocolDefinition::address_t address,
                                       ProtocolDefinition::sequence_t acknowledged_msg_sequence)
    : Message(address)
    , acknowledged_msg_sequence(acknowledged_msg_sequence) {
}

AcknowledgeMessage::AcknowledgeMessage(ProtocolDefinition::address_t address,
                                       ProtocolDefinition::sequence_t acknowledged_msg_sequence,
                                       MessageMetaData meta_data, ProtocolDefinition::sequence_t sequence)
    : Message(address, sequence, meta_data)
    , acknowledged_msg_sequence(acknowledged_msg_sequence) {
}

MessageType AcknowledgeMessage::message_type = MessageType::ACKNOWLEDGE;

MessageType AcknowledgeMessage::GetMessageType() const {
  return message_type;
}
ProtocolDefinition::sequence_t AcknowledgeMessage::GetAcknowledgedMessageSequence() const {
  return acknowledged_msg_sequence;
}
