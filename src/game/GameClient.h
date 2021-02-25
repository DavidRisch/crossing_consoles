#ifndef CROSSING_CONSOLES_GAMECLIENT_H
#define CROSSING_CONSOLES_GAMECLIENT_H

#include "Player.h"
#include "Renderer.h"
#include "World.h"

class GameClient {
 public:
  int viewport_size[2]{};
  Player player;
  World world;
  Renderer* renderer;
  int keypress = 0;

  GameClient(int viewport_width, int viewport_height, Player player, World world);

  void RunGame();
  void KeyPressed();
};

#endif  // CROSSING_CONSOLES_GAMECLIENT_H
