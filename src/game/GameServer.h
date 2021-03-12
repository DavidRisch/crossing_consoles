#ifndef CROSSING_CONSOLES_GAMESERVER_H
#define CROSSING_CONSOLES_GAMESERVER_H

#include <memory>

#include "World.h"

class GameServer {
 public:
  coordinate_size_t world_size;
  std::unique_ptr<World> world;

  explicit GameServer(coordinate_size_t world_size);

  [[noreturn]] void RunServer();
  void BroadcastMessage();
  void ReceiveMessages();
};

#endif  // CROSSING_CONSOLES_GAMESERVER_H
