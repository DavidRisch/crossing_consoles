#ifndef CROSSING_CONSOLES_GAMEMESSAGE_H
#define CROSSING_CONSOLES_GAMEMESSAGE_H

#include "Message.h"

typedef std::string world_t;

class GameMessage : public Message {
 public:
  GameMessage(ip_address_t&, ip_address_t&, double, world_t&);

  world_t GetWorld();

 private:
  world_t world;
};

#endif  // CROSSING_CONSOLES_GAMEMESSAGE_H
