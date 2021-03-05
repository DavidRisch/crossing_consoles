#include "Message.h"

#include <stdexcept>

using namespace communication;
using namespace communication::message_layer;

Message::Message(address_t address, ProtocolDefinition::sequence_t sequence)
    : address(address)
    , sequence(sequence) {
}

Message::Message(address_t address, ProtocolDefinition::sequence_t sequence, MessageMetaData meta_data)
    : address(address)
    , meta_data(meta_data)
    , sequence(sequence) {
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
ProtocolDefinition::sequence_t Message::GetMessageSequence() const {
  return sequence;
}
