#include "Message.h"

Message::Message(address_t address, MessageType message_type) {
}

Message::Message(address_t address, MessageType message_type, time_t timestamp_received, time_t timestamp_sent) {
}

address_t Message::GetAddress() const {
  return address;
}

time_t Message::GetTimestampReceived() const {
  return timestamp_received;
}

time_t Message::GetTimestampSent() const {
  return timestamp_sent;
}
MessageType Message::GetMessageType() const {
  return message_type;
}
