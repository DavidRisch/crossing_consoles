#ifndef CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
#define CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H

#include "Message.h"

namespace communication::message_layer {

class ConnectionRequestMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit ConnectionRequestMessage(ProtocolDefinition::address_t address);

  // Received message
  ConnectionRequestMessage(ProtocolDefinition::address_t address, ProtocolDefinition::sequence_t sequence);

  [[nodiscard]] MessageType GetMessageType() const override;

  // TODO: Protocol specification calls for 'client identification', that should happen here.
};

}  // namespace communication::message_layer

#endif  // CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
