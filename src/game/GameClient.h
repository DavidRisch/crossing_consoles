#ifndef CROSSING_CONSOLES_GAMECLIENT_H
#define CROSSING_CONSOLES_GAMECLIENT_H

#include <memory>

#include "Compositor.h"
#include "Player.h"
#include "World.h"
#include "terminal/ITerminal.h"

enum class KeyCode { ESCAPE = 27, W = 'w', A = 'a', S = 's', D = 'd', SPACE = ' ' };

class GameClient {
 public:
  GameClient(Player player, World world, std::shared_ptr<ITerminal> terminal);

  void RunGame();
  void ProcessInput();

 private:
  Player player;
  World world;
  std::shared_ptr<ITerminal> terminal;
  std::unique_ptr<Compositor> compositor;
  bool keep_running = true;
};

#endif  // CROSSING_CONSOLES_GAMECLIENT_H
