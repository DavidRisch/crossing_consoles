#ifndef CROSSING_CONSOLES_MESSAGE_H
#define CROSSING_CONSOLES_MESSAGE_H

#include <cstdint>
#include <string>
#include <vector>

#include "MessageMetaData.h"

enum MessageType : char { CONNECTION_REQUEST, CONNECTION_RESPONSE, PAYLOAD, KEEP_ALIVE };

typedef uint16_t address_t;

class Message {
 public:
  // Draft message
  Message(address_t address, MessageType message_type);

  // Received message
  Message(address_t address, MessageType message_type, MessageMetaData meta_data);

  [[nodiscard]] address_t GetAddress() const;
  [[nodiscard]] MessageType GetMessageType() const;
  [[nodiscard]] MessageMetaData GetMessageMetaData() const;

 private:
  address_t address;
  MessageMetaData meta_data = MessageMetaData(0, 0);
  MessageType message_type;
};

#endif  // CROSSING_CONSOLES_MESSAGE_H
