#include <cassert>
#include <csignal>
#include <iostream>

#include "game/GameClient.h"
#include "game/terminal/RealTerminal.h"
#include "game/world/Player.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::terminal;

GameClient *GAME_CLIENT;

void SignalHandler(int signal) {
  assert(signal == SIGINT);
  assert(GAME_CLIENT != nullptr);
  GAME_CLIENT->StartExit();
}

void SetupSignalHandler(GameClient *game_client) {
  GAME_CLIENT = game_client;

  struct sigaction sigIntHandler {};
  sigIntHandler.sa_handler = SignalHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, nullptr);
}

int main() {
  std::cout << "main client" << std::endl;
  auto player = std::make_shared<Player>("user", Position(0, 0));
  auto terminal = std::make_shared<RealTerminal>();
  GameClient game_client(player, terminal, coordinate_size_t(1, 1), true);

  SetupSignalHandler(&game_client);

  game_client.Run();
}
