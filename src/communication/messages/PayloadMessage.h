#ifndef CROSSING_CONSOLES_PAYLOADMESSAGE_H
#define CROSSING_CONSOLES_PAYLOADMESSAGE_H

#include "Message.h"

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

#endif  // CROSSING_CONSOLES_PAYLOADMESSAGE_H
