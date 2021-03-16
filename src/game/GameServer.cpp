#include "GameServer.h"

#include <cassert>
#include <iostream>
#include <thread>

#include "../communication/connection_layer/event/PayloadEvent.h"
#include "networking/Change.h"
#include "world/WorldGenerator.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::networking;

GameServer::GameServer(const coordinate_size_t &world_size) {
  world = WorldGenerator::GenerateWorld(world_size);

  server_manager = communication::connection_layer::ServerSideConnectionManager::CreateServerSide();
}

void GameServer::RunIteration() {
  server_manager->HandleConnections();
  auto event = server_manager->PopAndGetOldestEvent();
  if (event != nullptr) {
    HandleEvent(event);
  }

  if (std::chrono::steady_clock::now() - last_world_sent >= send_world_interval) {
    Change action(ChangeType::SET_WORLD);
    world->Serialize(action.payload);
    server_manager->Broadcast(action.payload);
  }
}

void GameServer::HandleEvent(const std::shared_ptr<communication::connection_layer::Event> &event) {
  switch (event->GetType()) {
    case communication::connection_layer::EventType::CONNECT: {
      assert(world->players.empty());  // TODO: support multiple players
      world->AddPlayer(std::make_shared<Player>("?", Position(0, 0)));
      break;
    }
    case communication::connection_layer::EventType::DISCONNECT: {
      assert(false);  // TODO: handle client disconnect
      break;
    }
    case communication::connection_layer::EventType::PAYLOAD: {
      Change change(std::dynamic_pointer_cast<communication::connection_layer::PayloadEvent>(event)->GetPayload());
      HandleChange(change);
      break;
    }
  }
}

void GameServer::HandleChange(const Change &change) {
  switch (change.GetChangeType()) {
    case ChangeType::MOVE_UP: {
      MovePlayer(coordinate_distance_t(0, -1));
      break;
    }
    case ChangeType::MOVE_LEFT: {
      MovePlayer(coordinate_distance_t(-1, 0));
      break;
    }
    case ChangeType::MOVE_RIGHT: {
      MovePlayer(coordinate_distance_t(1, 0));
      break;
    }
    case ChangeType::MOVE_DOWN: {
      MovePlayer(coordinate_distance_t(0, 1));
      break;
    }
    default:
      throw std::runtime_error("Unexpected ChangeType");
  }
}

void GameServer::MovePlayer(const coordinate_distance_t &movement) {  // NOLINT(readability-make-member-function-const)
  assert(world->players.size() == 1);  // TODO: more advanced setup require to handle multiple players
  auto &player = *world->players.front();

  Position new_position = player.position + movement;

  if (new_position.x < 0) {
    new_position.x += world->size.x;
  } else if (new_position.x >= world->size.x) {
    new_position.x -= world->size.x;
  }
  if (new_position.y < 0) {
    new_position.y += world->size.y;
  } else if (new_position.y >= world->size.y) {
    new_position.y -= world->size.y;
  }

  if (new_position != player.position && !world->IsBlocked(new_position)) {
    player.MoveTo(new_position);
  }
}
