#ifndef CROSSING_CONSOLES_RESETMESSAGE_H
#define CROSSING_CONSOLES_RESETMESSAGE_H

#include "Message.h"

namespace communication {
namespace message_layer {

class ResetMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit ResetMessage();

  // Received message
  explicit ResetMessage(ProtocolDefinition::sequence_t sequence);

  [[nodiscard]] MessageType GetMessageType() const override;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_RESETMESSAGE_H
