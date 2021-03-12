#include <iostream>

#include "game/GameServer.h"

int main() {
  std::cout << "main server" << std::endl;
  GameServer server(coordinate_size_t(100,50));
}
