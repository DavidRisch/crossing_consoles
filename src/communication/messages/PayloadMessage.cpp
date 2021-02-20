#include "PayloadMessage.h"

#include <utility>

PayloadMessage::PayloadMessage(address_t address, MessageMetaData meta_data)
    : Message(address, message_type, meta_data) {
}
PayloadMessage::PayloadMessage(address_t address)
    : Message(address, message_type) {
}

MessageType PayloadMessage::message_type = PAYLOAD;
