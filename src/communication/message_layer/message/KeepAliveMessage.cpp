#include "KeepAliveMessage.h"

KeepAliveMessage::KeepAliveMessage(address_t address, MessageMetaData meta_data)
    : Message(address, meta_data) {
}
KeepAliveMessage::KeepAliveMessage(address_t address)
    : Message(address) {
}

MessageType KeepAliveMessage::message_type = MessageType::KEEP_ALIVE;

MessageType KeepAliveMessage::GetMessageType() const {
  return message_type;
}
