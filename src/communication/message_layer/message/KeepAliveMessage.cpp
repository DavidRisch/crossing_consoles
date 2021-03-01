#include "KeepAliveMessage.h"

using namespace communication;
using namespace communication::message_layer;

KeepAliveMessage::KeepAliveMessage(address_t address, MessageMetaData meta_data)
    : Message(address, meta_data) {
}
KeepAliveMessage::KeepAliveMessage(address_t address)
    : Message(address) {
}

MessageType KeepAliveMessage::message_type = MessageType::KEEP_ALIVE;

MessageType KeepAliveMessage::GetMessageType() const {
  return message_type;
}
