#include "ConnectionResponseMessage.h"

ConnectionResponseMessage::ConnectionResponseMessage(address_t address, MessageMetaData meta_data)
    : Message(address, message_type, meta_data) {
}
ConnectionResponseMessage::ConnectionResponseMessage(address_t address)
    : Message(address, message_type) {
}

MessageType ConnectionResponseMessage::message_type = CONNECTION_RESPONSE;
