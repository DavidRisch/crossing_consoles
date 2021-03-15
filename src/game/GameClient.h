#ifndef CROSSING_CONSOLES_GAMECLIENT_H
#define CROSSING_CONSOLES_GAMECLIENT_H

#include <memory>

#include "../communication/connection_layer/connection/ClientSideConnectionManager.h"
#include "terminal/ITerminal.h"
#include "visual/Compositor.h"
#include "world/Player.h"
#include "world/World.h"

namespace game {

enum class KeyCode { ESCAPE = 27, W = 'w', A = 'a', S = 's', D = 'd', SPACE = ' ' };

class GameClient {
 public:
  GameClient(world::Player player, std::shared_ptr<terminal::ITerminal> terminal,
             const common::coordinate_size_t& world, bool multiplayer = false);

  void Run();
  void ProcessInput();

 private:
  world::Player player;
  world::World world;
  std::shared_ptr<terminal::ITerminal> terminal;
  std::unique_ptr<visual::Compositor> compositor;
  std::shared_ptr<communication::connection_layer::ClientSideConnectionManager> client_manager;
  bool keep_running = true;
  bool multiplayer;
};

}  // namespace game

#endif  // CROSSING_CONSOLES_GAMECLIENT_H
