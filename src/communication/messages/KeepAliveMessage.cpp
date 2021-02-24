#include "KeepAliveMessage.h"

KeepAliveMessage::KeepAliveMessage(address_t address, MessageMetaData meta_data)
    : Message(address, message_type, meta_data) {
}
KeepAliveMessage::KeepAliveMessage(address_t address)
    : Message(address, message_type) {
}

MessageType KeepAliveMessage::message_type = KEEP_ALIVE;
