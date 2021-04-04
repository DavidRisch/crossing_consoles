#ifndef CROSSING_CONSOLES_PAYLOADMESSAGE_H
#define CROSSING_CONSOLES_PAYLOADMESSAGE_H

#include "Message.h"

namespace communication {
namespace message_layer {

/**
 * \brief Message containing data/payload.
 */
class PayloadMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit PayloadMessage(std::vector<uint8_t> payload);

  // Received message
  PayloadMessage(std::vector<uint8_t> payload, ProtocolDefinition::sequence_t sequence);

  [[nodiscard]] MessageType GetMessageType() const override;
  [[nodiscard]] std::vector<uint8_t> GetPayload() const;

 private:
  std::vector<uint8_t> payload;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_PAYLOADMESSAGE_H
