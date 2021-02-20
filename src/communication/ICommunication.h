#ifndef CROSSING_CONSOLES_COMMUNICATIONINTERFACE_H
#define CROSSING_CONSOLES_COMMUNICATIONINTERFACE_H

#include <cstdint>

#include "messages/Message.h"

class ICommunication {
 public:
  virtual void Send(const Message &) = 0;
  virtual Message Receive() = 0;
};

#endif  // CROSSING_CONSOLES_COMMUNICATIONINTERFACE_H
