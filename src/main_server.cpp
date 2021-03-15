#include <iostream>
#include <thread>

#include "game/GameServer.h"

int main() {
  std::cout << "main server" << std::endl;
  GameServer server(coordinate_size_t(20, 15));
  while (true) {
    server.RunIteration();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));  // TODO: shorter sleep
  }
}
