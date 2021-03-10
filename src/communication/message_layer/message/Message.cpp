#include "Message.h"

#include <stdexcept>

using namespace communication;
using namespace communication::message_layer;

Message::Message(ProtocolDefinition::address_t address)
    : address(address) {
  sequence = 0;
  meta_data = MessageMetaData();
}

Message::Message(ProtocolDefinition::address_t address, ProtocolDefinition::sequence_t sequence)
    : address(address)
    , sequence(sequence) {
  meta_data = MessageMetaData();
}

ProtocolDefinition::address_t Message::GetAddress() const {
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
void Message::SetMessageSequence(ProtocolDefinition::sequence_t new_sequence) {
  sequence = new_sequence;
}

void Message::SetTimestampReceived(std::chrono::steady_clock::time_point timestamp) {
  meta_data.SetTimestampReceived(timestamp);
}

void Message::SetTimestampSent(std::chrono::steady_clock::time_point timestamp) {
  meta_data.SetTimestampSent(timestamp);
}
