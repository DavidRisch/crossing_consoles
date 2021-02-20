#ifndef CROSSING_CONSOLES_KEEPALIVEMESSAGE_H
#define CROSSING_CONSOLES_KEEPALIVEMESSAGE_H

#include "Message.h"

class KeepAliveMessage : public Message {
 public:
  static MessageType message_type;
  explicit KeepAliveMessage(address_t address);

  // Received message
  KeepAliveMessage(address_t address, MessageMetaData meta_data);
};

#endif  // CROSSING_CONSOLES_KEEPALIVEMESSAGE_H
