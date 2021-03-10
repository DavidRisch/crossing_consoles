#ifndef CROSSING_CONSOLES_ACKNOWLEDGEMESSAGE_H
#define CROSSING_CONSOLES_ACKNOWLEDGEMESSAGE_H

#include "../../ProtocolDefinition.h"
#include "Message.h"

namespace communication::message_layer {

class AcknowledgeMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit AcknowledgeMessage(ProtocolDefinition::address_t address,
                              ProtocolDefinition::sequence_t acknowledged_msg_sequence);

  // Received message
  AcknowledgeMessage(ProtocolDefinition::address_t address, ProtocolDefinition::sequence_t acknowledged_msg_sequence,
                     MessageMetaData meta_data, ProtocolDefinition::sequence_t sequence = 0);

  [[nodiscard]] MessageType GetMessageType() const override;

  [[nodiscard]] ProtocolDefinition::sequence_t GetAcknowledgedMessageSequence() const;

 private:
  ProtocolDefinition::sequence_t acknowledged_msg_sequence;
};

}  // namespace communication::message_layer

#endif  // CROSSING_CONSOLES_ACKNOWLEDGEMESSAGE_H
