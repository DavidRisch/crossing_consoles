#include "ResetMessage.h"

using namespace communication;
using namespace communication::message_layer;

ResetMessage::ResetMessage() = default;

ResetMessage::ResetMessage(communication::ProtocolDefinition::sequence_t sequence)
    : Message(sequence) {
}

MessageType ResetMessage::message_type = MessageType::KEEP_ALIVE;

MessageType ResetMessage::GetMessageType() const {
  return message_type;
}
