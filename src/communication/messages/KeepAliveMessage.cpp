#include "KeepAliveMessage.h"

KeepAliveMessage::KeepAliveMessage(address_t address, time_t timestamp_received, time_t timestamp_sent)
    : Message(address, message_type, timestamp_received, timestamp_sent) {
}
KeepAliveMessage::KeepAliveMessage(address_t address)
    : Message(address, message_type) {
}

MessageType KeepAliveMessage::message_type = KEEP_ALIVE;
