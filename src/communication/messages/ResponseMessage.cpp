#include "ResponseMessage.h"

ResponseMessage::ResponseMessage(ip_address_t& source, ip_address_t& destination, double timestamp, bool status,
                                 std::string& error)
    : Message(source, destination, timestamp) {
  this->status = status;
  this->error = error;
}

bool ResponseMessage::GetStatus() const {
  return this->status;
}
std::string ResponseMessage::GetError() {
  return this->error;
}


