#ifndef CROSSING_CONSOLES_GAMECOMMANDMESSAGE_H
#define CROSSING_CONSOLES_GAMECOMMANDMESSAGE_H

#include "Message.h"

typedef std::string command_t;

class GameCommandMessage : public Message {
 public:
  GameCommandMessage(ip_address_t&, ip_address_t&, double, command_t&);

  command_t GetCommand();

 private:
  command_t command;
};

#endif  // CROSSING_CONSOLES_GAMECOMMANDMESSAGE_H
