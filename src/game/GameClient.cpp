#include "GameClient.h"

#include <chrono>
#include <thread>
#include <utility>

enum Keys : int { KEY_ESCAPE = 27, KEY_W = 'w', KEY_A = 'a', KEY_S = 's', KEY_D = 'd', KEY_SPACE = ' ' };

GameClient::GameClient(Player player, World world, std::shared_ptr<ITerminal> terminal)
    : player(std::move(player))
    , world(std::move(world))
    , terminal(std::move(terminal)) {
  this->world.AddPlayer(&this->player);
  coordinate_size_t viewport_size = Position(51, 31);
  compositor = new Compositor(viewport_size, this->world, this->player);

  RunGame();
}

void GameClient::RunGame() {
  while (keypress != KEY_ESCAPE) {
    ProcessInput();
    if (world.updated || player.updated) {
      terminal->SetScreen(compositor->CompositeViewport());
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
  }
}

void GameClient::ProcessInput() {
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
      Position new_position = player.position + movement;

      if (new_position.x < 0) {
        new_position.x += world.size.x;
      } else if (new_position.x >= world.size.x) {
        new_position.x -= world.size.x;
      }
      if (new_position.y < 0) {
        new_position.y += world.size.y;
      } else if (new_position.y >= world.size.y) {
        new_position.y -= world.size.y;
      }

      if (new_position != player.position && !world.IsWall(new_position)) {
        player.MoveTo(new_position);
      }
    }
  }
}