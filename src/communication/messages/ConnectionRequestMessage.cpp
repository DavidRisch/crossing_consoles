#include "ConnectionRequestMessage.h"

ConnectionRequestMessage::ConnectionRequestMessage(address_t address, time_t timestamp_received, time_t timestamp_sent)
    : Message(address, message_type, timestamp_received, timestamp_sent) {
}
ConnectionRequestMessage::ConnectionRequestMessage(address_t address)
    : Message(address, message_type) {
}

MessageType ConnectionRequestMessage::message_type = CONNECTION_REQUEST;
