#include "ConnectionRequestMessage.h"

ConnectionRequestMessage::ConnectionRequestMessage(address_t address, MessageMetaData meta_data)
    : Message(address, meta_data) {
}
ConnectionRequestMessage::ConnectionRequestMessage(address_t address)
    : Message(address) {
}

MessageType ConnectionRequestMessage::message_type = MessageType::CONNECTION_REQUEST;

MessageType ConnectionRequestMessage::GetMessageType() const {
  return message_type;
}
