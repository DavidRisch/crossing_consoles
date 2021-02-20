#include "Message.h"

Message::Message(address_t address, MessageType message_type_de) {
  this->address= address;
  message_type = message_type_de;
}

Message::Message(address_t address, MessageType message_type, MessageMetaData meta_data) {
  this->address = address;
  this->message_type = message_type;
  this->meta_data = meta_data;

}

address_t Message::GetAddress() const {
  return address;
}

MessageType Message::GetMessageType() const {
  return message_type;
}

MessageMetaData Message::GetMessageMetaData() const{
  return meta_data;
}
