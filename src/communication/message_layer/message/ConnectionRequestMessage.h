#ifndef CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
#define CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H

#include "Message.h"

namespace communication::message_layer {

/**
 * \brief Sent by the Client to the Server to request a connection and initiate the handshake.
 */
class ConnectionRequestMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit ConnectionRequestMessage();

  // Received message
  explicit ConnectionRequestMessage(ProtocolDefinition::sequence_t sequence);

  [[nodiscard]] MessageType GetMessageType() const override;

  // TODO: Protocol specification calls for 'client identification', that should happen here.
};

}  // namespace communication::message_layer

#endif  // CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
