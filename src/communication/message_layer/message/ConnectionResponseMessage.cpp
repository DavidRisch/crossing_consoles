#include "ConnectionResponseMessage.h"

using namespace communication;
using namespace communication::message_layer;

ConnectionResponseMessage::ConnectionResponseMessage(address_t address, MessageMetaData meta_data)
    : Message(address, meta_data) {
}
ConnectionResponseMessage::ConnectionResponseMessage(address_t address)
    : Message(address) {
}

MessageType ConnectionResponseMessage::message_type = MessageType::CONNECTION_RESPONSE;

MessageType ConnectionResponseMessage::GetMessageType() const {
  return message_type;
}
