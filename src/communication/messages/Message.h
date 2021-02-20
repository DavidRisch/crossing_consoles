#ifndef CROSSING_CONSOLES_MESSAGE_H
#define CROSSING_CONSOLES_MESSAGE_H

#include <cstdint>
#include <string>
#include <vector>

enum MessageType : char { CONNECTION_REQUEST, CONNECTION_RESPONSE, PAYLOAD, KEEP_ALIVE};

typedef uint16_t address_t;

class Message {
 public:
  // Draft message
  Message(address_t address, MessageType message_type);

  // Received message
  Message(address_t address, MessageType message_type, time_t timestamp_received, time_t timestamp_sent);

  [[nodiscard]] time_t GetTimestampReceived() const;
  [[nodiscard]] time_t GetTimestampSent() const;
  [[nodiscard]] address_t GetAddress() const;
  [[nodiscard]] MessageType GetMessageType() const;

 private:
  address_t address;
  time_t timestamp_received;
  time_t timestamp_sent;
  MessageType message_type;
};

#endif  // CROSSING_CONSOLES_MESSAGE_H