#include "GameClient.h"

#include <conio.h>

#include <iostream>
#include <string>
#include <utility>

enum Keys : int { KEY_ESCAPE = 27, KEY_W = 119, KEY_A = 97, KEY_S = 115, KEY_D = 100 };

GameClient::GameClient(int viewport_width, int viewport_height, Player player, World world)
    : player(std::move(player))
    , world(std::move(world)) {
  viewport_size[0] = viewport_width;
  viewport_size[1] = viewport_height;
  renderer = new Renderer(viewport_size[0], viewport_size[1], this->world, this->player);
  RunGame();
}

void GameClient::RunGame() {
  while (keypress != KEY_ESCAPE) {
    KeyPressed();
    if (world.updated || player.updated) {
      system("cls");
      std::cout << renderer->RenderWorld() << std::endl;
    }
  }
}

void GameClient::KeyPressed() {
  if (kbhit()) {
    keypress = getch();
    switch (keypress) {
      case KEY_W:
        player.MoveBy(Position(0, -1));
        break;
      case KEY_A:
        player.MoveBy(Position(-1, 0));
        break;
      case KEY_S:
        player.MoveBy(Position(0, 1));
        break;
      case KEY_D:
        player.MoveBy(Position(1, 0));
        break;
    }
  }
}