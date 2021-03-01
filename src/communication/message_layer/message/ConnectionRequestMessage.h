#ifndef CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
#define CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H

#include "Message.h"

namespace communication {
namespace message_layer {

class ConnectionRequestMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit ConnectionRequestMessage(address_t address);

  // Received message
  ConnectionRequestMessage(address_t address, MessageMetaData meta_data);

  [[nodiscard]] MessageType GetMessageType() const override;

  // TODO: Protocol specification calls for 'client identification', that should happen here.
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CONNECTIONREQUESTMESSAGE_H
