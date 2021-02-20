#ifndef CROSSING_CONSOLES_PAYLOADMESSAGE_H
#define CROSSING_CONSOLES_PAYLOADMESSAGE_H

#include "Message.h"

typedef std::string world_t;

class PayloadMessage : public Message {
 public:
  static MessageType message_type;
  explicit PayloadMessage(address_t address);

  // Received message
  PayloadMessage(address_t address, MessageMetaData meta_data);
};

#endif  // CROSSING_CONSOLES_PAYLOADMESSAGE_H
