#ifndef CROSSING_CONSOLES_RESPONSEMESSAGE_H
#define CROSSING_CONSOLES_RESPONSEMESSAGE_H

#include "Message.h"

class ResponseMessage : public Message {
 public:
  ResponseMessage(ip_address_t&, ip_address_t&, double, bool, std::string&);

  bool GetStatus() const;
  std::string GetError();

 private:
  bool status;
  std::string error;
};

#endif  // CROSSING_CONSOLES_RESPONSEMESSAGE_H
