#ifndef CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
#define CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H

#include "Message.h"

namespace communication::message_layer {

class ConnectionRequestMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit ConnectionRequestMessage(address_t address, ProtocolDefinition::sequence_t sequence = 0);

  // Received message
  ConnectionRequestMessage(address_t address, MessageMetaData meta_data, ProtocolDefinition::sequence_t sequence = 0);

  [[nodiscard]] MessageType GetMessageType() const override;

  // TODO: Protocol specification calls for 'client identification', that should happen here.
};

}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
