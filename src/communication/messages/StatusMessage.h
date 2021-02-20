#ifndef CROSSING_CONSOLES_STATUSMESSAGE_H
#define CROSSING_CONSOLES_STATUSMESSAGE_H

#include "Message.h"

class StatusMessage : public Message {
 public:
  StatusMessage(ip_address_t&, ip_address_t&, double);
};

#endif  // CROSSING_CONSOLES_STATUSMESSAGE_H
