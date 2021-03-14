#include "GameClient.h"

#include <chrono>
#include <thread>
#include <utility>

GameClient::GameClient(Player player, World world, std::shared_ptr<ITerminal> terminal)
    : player(std::move(player))
    , world(std::move(world))
    , terminal(std::move(terminal)) {
  this->world.AddPlayer(&this->player);
  coordinate_size_t viewport_size = Position(51, 31);
  compositor = std::make_unique<Compositor>(viewport_size, this->world, this->player);

  RunGame();
}

void GameClient::RunGame() {
  while (keep_running) {
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
    int keypress = terminal->GetInput();
    switch (static_cast<KeyCode>(keypress)) {
      case KeyCode::W:
        movement.Set(0, -1);
        break;
      case KeyCode::A:
        movement.Set(-1, 0);
        break;
      case KeyCode::S:
        movement.Set(0, 1);
        break;
      case KeyCode::D:
        movement.Set(1, 0);
        break;
      case KeyCode::ESCAPE:
        keep_running = false;
      default:
        return;
    }

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

    if (new_position != player.position && !world.IsBlocked(new_position)) {
      player.MoveTo(new_position);
    }
  }
}
