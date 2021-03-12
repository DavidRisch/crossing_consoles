#include "GameServer.h"

#include <iostream>

GameServer::GameServer(coordinate_size_t world_size)
    : world_size(world_size) {
  world = std::make_unique<World>(world_size);
  RunServer();
}

[[noreturn]] void GameServer::RunServer() {
  while (true) {
    std::cout << "running...\n";
  }
}

void GameServer::BroadcastMessage() {
}

void GameServer::ReceiveMessages() {
}