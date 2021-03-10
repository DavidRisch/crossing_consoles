#include "ConnectionResetMessage.h"

using namespace communication;
using namespace communication::message_layer;

ConnectionResetMessage::ConnectionResetMessage()
    : Message() {
}

ConnectionResetMessage::ConnectionResetMessage(communication::ProtocolDefinition::sequence_t sequence)
    : Message(sequence) {
}

MessageType ConnectionResetMessage::message_type = MessageType::KEEP_ALIVE;

MessageType ConnectionResetMessage::GetMessageType() const {
  return message_type;
}
