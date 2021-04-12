#ifndef CROSSING_CONSOLES_NOT_ACKNOWLEDGE_MESSAGE_H
#define CROSSING_CONSOLES_NOT_ACKNOWLEDGE_MESSAGE_H

#include "Message.h"

namespace communication::message_layer {

/**
 * \brief Sent if the receiving of a `Message` failed.
 */
class NotAcknowledgeMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  NotAcknowledgeMessage();

  // Received message
  explicit NotAcknowledgeMessage(ProtocolDefinition::sequence_t sequence);

  [[nodiscard]] MessageType GetMessageType() const override;
};

}  // namespace communication::message_layer

#endif  // CROSSING_CONSOLES_NOT_ACKNOWLEDGE_MESSAGE_H
