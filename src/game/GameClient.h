#ifndef CROSSING_CONSOLES_GAMECLIENT_H
#define CROSSING_CONSOLES_GAMECLIENT_H

#include <memory>

#include "Compositor.h"
#include "Player.h"
#include "World.h"
#include "terminal/ITerminal.h"

class GameClient {
 public:
  Player player;
  World world;
  Compositor* compositor;
  int keypress = 0;

  GameClient(Player player, World world, std::shared_ptr<ITerminal> terminal);

  void RunGame();
  void KeyPressed();

 private:
  std::shared_ptr<ITerminal> terminal;
};

#endif  // CROSSING_CONSOLES_GAMECLIENT_H
