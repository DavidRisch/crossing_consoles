#include "ConnectionResponseMessage.h"

ConnectionResponseMessage::ConnectionResponseMessage(address_t address, time_t timestamp_received,
                                                     time_t timestamp_sent)
    : Message(address, message_type, timestamp_received, timestamp_sent) {
}
ConnectionResponseMessage::ConnectionResponseMessage(address_t address)
    : Message(address, message_type) {
}

MessageType ConnectionResponseMessage::message_type = CONNECTION_RESPONSE;
