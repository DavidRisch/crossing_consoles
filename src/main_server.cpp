#include <iostream>
#include <thread>

#include "game/GameServer.h"

using namespace game;
using namespace game::common;

int main(int argc, char *argv[]) {
  coordinate_size_t world_size(100, 100);

  if (argc == 3) {
    world_size.x = std::stoi(argv[1]);
    assert(world_size.x > 0);
    world_size.y = std::stoi(argv[2]);
    assert(world_size.y > 0);
  }

  std::cout << "Starting server..." << std::endl;
  GameServer server(world_size);
  std::cout << "Server ready" << std::endl;
  while (true) {
    server.RunIteration(true);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
}
