#include <iostream>

#include "game/GameClient.h"
#include "game/terminal/RealTerminal.h"
#include "game/world/Player.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::terminal;

int main() {
  std::cout << "main client" << std::endl;
  auto player = std::make_shared<Player>("user", Position(0, 0));
  auto terminal = std::make_shared<RealTerminal>();
  GameClient game_client(player, terminal, coordinate_size_t(100, 100), true);
  game_client.Run();
}
