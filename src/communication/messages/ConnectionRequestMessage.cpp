#include "ConnectionRequestMessage.h"

ConnectionRequestMessage::ConnectionRequestMessage(address_t address, MessageMetaData meta_data)
    : Message(address, message_type, meta_data) {
}
ConnectionRequestMessage::ConnectionRequestMessage(address_t address)
    : Message(address, message_type) {
}

MessageType ConnectionRequestMessage::message_type = CONNECTION_REQUEST;
