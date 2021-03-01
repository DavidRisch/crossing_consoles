#ifndef CROSSING_CONSOLES_MESSAGE_H
#define CROSSING_CONSOLES_MESSAGE_H

#include <cstdint>
#include <string>
#include <vector>

#include "MessageMetaData.h"

namespace communication {
namespace message_layer {

enum class MessageType : char {
  CONNECTION_REQUEST = 0,
  CONNECTION_RESPONSE,
  PAYLOAD,
  KEEP_ALIVE,
  HIGHEST_ELEMENT = KEEP_ALIVE
};

typedef uint16_t address_t;

class Message {
 public:
  // Draft message
  Message(address_t address);

  // Received message
  Message(address_t address, MessageMetaData meta_data);

  [[nodiscard]] address_t GetAddress() const;
  [[nodiscard]] virtual MessageType GetMessageType() const;
  [[nodiscard]] MessageMetaData GetMessageMetaData() const;

 private:
  address_t address;
  MessageMetaData meta_data = MessageMetaData(0, 0);
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_MESSAGE_H
