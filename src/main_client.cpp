#include <iostream>

#include "game/GameClient.h"
#include "game/Player.h"
#include "game/World.h"
#include "game/terminal/RealTerminal.h"

int main() {
  std::cout << "main client" << std::endl;
  World world(coordinate_size_t(20, 15));
  world.AddWall(Position(1, 1));
  world.AddWall(Position(2, 1));
  world.AddWall(Position(3, 1));
  world.AddWall(Position(3, 2));
  Player player("user", Position(0, 0));
  auto terminal = std::make_shared<RealTerminal>();
  GameClient game_client(player, world, terminal, true);
  game_client.Run();
}
