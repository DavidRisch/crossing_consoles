#include <iostream>

#include "game/GameClient.h"
#include "game/terminal/RealTerminal.h"
#include "game/world/Player.h"

int main() {
  std::cout << "main client" << std::endl;
  Player player("user", Position(0, 0));
  auto terminal = std::make_shared<RealTerminal>();
  GameClient game_client(player, terminal, coordinate_size_t(1, 1), true);
  game_client.Run();
}
