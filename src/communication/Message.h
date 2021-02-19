#ifndef CROSSING_CONSOLES_MESSAGE_H
#define CROSSING_CONSOLES_MESSAGE_H

#include <cstdint>
#include <vector>
enum MessageType : char { KEEPALIVE, RESET, REQUEST, RESPONSE, ACK_RECEIVED, UPDATES };

typedef uint8_t checksum_t;

class Message {
 public:
  Message();

  std::vector<uint8_t> GetContent();
  MessageType GetMessageType();
  checksum_t Get_Checksum();

 private:
  std::vector<uint8_t> content;  // TODO: size of array?
  MessageType message_type;
  checksum_t checksum;
};

#endif  // CROSSING_CONSOLES_MESSAGE_H
