#ifndef CROSSING_CONSOLES_CONNECTIONRESETMESSAGE_H
#define CROSSING_CONSOLES_CONNECTIONRESETMESSAGE_H

#include "Message.h"

namespace communication {
namespace message_layer {

/**
 * \brief Sent if one communication partner wants to close the `Connection`.
 */
class ConnectionResetMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit ConnectionResetMessage();

  // Received message
  explicit ConnectionResetMessage(ProtocolDefinition::sequence_t sequence);

  [[nodiscard]] MessageType GetMessageType() const override;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTIONRESETMESSAGE_H
