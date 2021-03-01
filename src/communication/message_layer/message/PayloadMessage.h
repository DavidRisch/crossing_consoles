#ifndef CROSSING_CONSOLES_PAYLOADMESSAGE_H
#define CROSSING_CONSOLES_PAYLOADMESSAGE_H

#include "Message.h"

namespace communication {
namespace message_layer {

class PayloadMessage : public Message {
 public:
  static MessageType message_type;

  // Draft message
  explicit PayloadMessage(address_t address, std::vector<uint8_t> payload);

  // Received message
  PayloadMessage(address_t address, std::vector<uint8_t> payload, MessageMetaData meta_data);

  [[nodiscard]] MessageType GetMessageType() const override;
  [[nodiscard]] std::vector<uint8_t> GetPayload() const;

 private:
  std::vector<uint8_t> payload;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_PAYLOADMESSAGE_H
