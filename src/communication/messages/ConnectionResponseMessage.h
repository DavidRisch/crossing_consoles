#ifndef CROSSING_CONSOLES_CONNECTIONRESPONSEMESSAGE_H
#define CROSSING_CONSOLES_CONNECTIONRESPONSEMESSAGE_H

#include "Message.h"

class ConnectionResponseMessage : public Message {
 public:
  static MessageType message_type;
  explicit ConnectionResponseMessage(address_t address);

  // Received message
  ConnectionResponseMessage(address_t address, time_t timestamp_received, time_t timestamp_sent);
};

#endif  // CROSSING_CONSOLES_CONNECTIONRESPONSEMESSAGE_H
