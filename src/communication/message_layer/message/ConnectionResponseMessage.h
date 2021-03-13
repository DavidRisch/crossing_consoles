#ifndef CROSSING_CONSOLES_CONNECTIONRESPONSEMESSAGE_H
#define CROSSING_CONSOLES_CONNECTIONRESPONSEMESSAGE_H

#include "Message.h"

namespace communication::message_layer {

class ConnectionResponseMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit ConnectionResponseMessage();

  // Received message
  explicit ConnectionResponseMessage(ProtocolDefinition::sequence_t sequence);

  [[nodiscard]] MessageType GetMessageType() const override;
};

}  // namespace communication::message_layer

#endif  // CROSSING_CONSOLES_CONNECTIONRESPONSEMESSAGE_H
