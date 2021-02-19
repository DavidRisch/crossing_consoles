#include "Message.h"

Message::Message() {
}

std::vector<uint8_t> Message::GetContent() {
  return std::vector<uint8_t>();
}

MessageType Message::GetMessageType() {
  return RESET;
}

uint8_t Message::Get_Checksum() {
  return 0;
}
