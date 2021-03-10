#ifndef CROSSING_CONSOLES_KEEPALIVEMESSAGE_H
#define CROSSING_CONSOLES_KEEPALIVEMESSAGE_H

#include "Message.h"

namespace communication {
namespace message_layer {

class KeepAliveMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit KeepAliveMessage(ProtocolDefinition::address_t address);

  // Received message
  KeepAliveMessage(ProtocolDefinition::address_t address, ProtocolDefinition::sequence_t sequence);

  [[nodiscard]] MessageType GetMessageType() const override;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_KEEPALIVEMESSAGE_H
