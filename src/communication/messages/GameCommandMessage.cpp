#include "GameCommandMessage.h"

GameCommandMessage::GameCommandMessage(ip_address_t& source, ip_address_t& destination, double timestamp,
                                       command_t& command)
    : Message(source, destination, timestamp) {
  this->command = command;
}

command_t GameCommandMessage::GetCommand() {
  return this->command;
}
