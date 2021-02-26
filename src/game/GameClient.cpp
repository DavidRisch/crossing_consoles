#include "GameClient.h"

#include <iostream>
#include <string>
#include <utility>

#include "terminal/RealTerminal.h"

enum Keys : int { KEY_ESCAPE = 27, KEY_W = 'w', KEY_A = 'a', KEY_S = 's', KEY_D = 'd', KEY_SPACE = ' ' };

GameClient::GameClient(Player player, World world, std::shared_ptr<ITerminal> terminal)
    : player(std::move(player))
    , world(std::move(world))
    , terminal(std::move(terminal)) {
  coordinate_size_t viewport_size = Position(51, 31);
  compositor = new Compositor(viewport_size, this->world, this->player);

  RunGame();
}

void GameClient::RunGame() {
  while (keypress != KEY_ESCAPE) {
    KeyPressed();
    if (world.updated || player.updated) {
      terminal->SetScreen(compositor->CompositeViewport());
    }
  }
}

void GameClient::KeyPressed() {
  coordinate_distance_t movement(0, 0);

  if (terminal->HasInput()) {
    keypress = terminal->GetInput();
    switch (keypress) {
      case KEY_W:
        movement.Set(0, -1);
        break;
      case KEY_A:
        movement.Set(-1, 0);
        break;
      case KEY_S:
        movement.Set(0, 1);
        break;
      case KEY_D:
        movement.Set(1, 0);
        break;
    }

    if (keypress) {
      if (movement != Position(0, 0) && !world.IsWall(player.position + movement)) {
        player.MoveBy(movement);
      }
    }
  }
}