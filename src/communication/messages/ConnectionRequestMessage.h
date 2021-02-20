#ifndef CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
#define CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H

#include "Message.h"

class ConnectionRequestMessage : public Message {
 public:
  static MessageType message_type;
  explicit ConnectionRequestMessage(address_t address);

  // Received message
  ConnectionRequestMessage(address_t address, time_t timestamp_received, time_t timestamp_sent);
};

#endif  // CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
