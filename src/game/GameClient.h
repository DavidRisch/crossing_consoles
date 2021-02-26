#ifndef CROSSING_CONSOLES_GAMECLIENT_H
#define CROSSING_CONSOLES_GAMECLIENT_H

#include "Player.h"
#include "Compositor.h"
#include "World.h"

class GameClient {
 public:
  Player player;
  World world;
  Compositor* compositor;
  int keypress = 0;

  GameClient(Player player, World world);

  void RunGame();
  void KeyPressed();
};

#endif  // CROSSING_CONSOLES_GAMECLIENT_H
