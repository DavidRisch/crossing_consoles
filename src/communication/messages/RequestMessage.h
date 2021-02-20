#ifndef CROSSING_CONSOLES_REQUESTMESSAGE_H
#define CROSSING_CONSOLES_REQUESTMESSAGE_H

#include "Message.h"

class RequestMessage: public Message {
 public:
  RequestMessage(ip_address_t&, ip_address_t&, double);
};

#endif  // CROSSING_CONSOLES_REQUESTMESSAGE_H
