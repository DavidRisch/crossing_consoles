#include "PayloadMessage.h"

#include <utility>

PayloadMessage::PayloadMessage(address_t address, time_t timestamp_received, time_t timestamp_sent)
    : Message(address, message_type, timestamp_received, timestamp_sent) {
}
PayloadMessage::PayloadMessage(address_t address)
    : Message(address, message_type) {
}

MessageType PayloadMessage::message_type = PAYLOAD;
