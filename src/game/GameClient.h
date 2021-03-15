#ifndef CROSSING_CONSOLES_GAMECLIENT_H
#define CROSSING_CONSOLES_GAMECLIENT_H

#include <memory>

#include "../communication/connection_layer/connection/ClientSideConnectionManager.h"
#include "Compositor.h"
#include "Player.h"
#include "World.h"
#include "terminal/ITerminal.h"

enum class KeyCode { ESCAPE = 27, W = 'w', A = 'a', S = 's', D = 'd', SPACE = ' ' };

class GameClient {
 public:
  GameClient(Player player, std::shared_ptr<ITerminal> terminal, const coordinate_size_t& world,
             bool multiplayer = false);

  void Run();
  void ProcessInput();

 private:
  Player player;
  World world;
  std::shared_ptr<ITerminal> terminal;
  std::unique_ptr<Compositor> compositor;
  std::shared_ptr<communication::connection_layer::ClientSideConnectionManager> client_manager;
  bool keep_running = true;
  bool multiplayer;
};

#endif  // CROSSING_CONSOLES_GAMECLIENT_H
