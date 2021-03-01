#include "Message.h"

#include <stdexcept>

using namespace communication;
using namespace communication::message_layer;

Message::Message(address_t address)
    : address(address) {
}

Message::Message(address_t address, MessageMetaData meta_data)
    : address(address)
    , meta_data(meta_data) {
}

address_t Message::GetAddress() const {
  return address;
}

MessageType Message::GetMessageType() const {
  throw std::logic_error("Not implemented");
}

MessageMetaData Message::GetMessageMetaData() const {
  return meta_data;
}
