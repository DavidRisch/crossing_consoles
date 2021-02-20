#include "Message.h"

Message::Message(ip_address_t& source, ip_address_t& destination, double timestamp) {
  this->source = source;
  this->destination = destination;
  this->timestamp = timestamp;
}

std::string Message::GetSource() {
  return this->source;
}

std::string Message::GetDestination() {
  return this->destination;
}

double Message::GetTimestamp() const {
  return this->timestamp;
}
