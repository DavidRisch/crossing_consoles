#ifndef CROSSING_CONSOLES_KEEPALIVEMESSAGE_H
#define CROSSING_CONSOLES_KEEPALIVEMESSAGE_H

#include "Message.h"

class KeepAliveMessage : public Message {
 public:
  static MessageType message_type;
  explicit KeepAliveMessage(address_t address);

  // Received message
  KeepAliveMessage(address_t address, time_t timestamp_received, time_t timestamp_sent);
};

#endif  // CROSSING_CONSOLES_KEEPALIVEMESSAGE_H
