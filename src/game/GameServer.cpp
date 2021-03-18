#include "GameServer.h"

#include <algorithm>
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

GameServer::GameServer(const coordinate_size_t &world_size,
                       communication::ProtocolDefinition::timeout_t communication_timeout) {
  world = WorldGenerator::GenerateWorld(world_size);

  server_manager =
      communication::connection_layer::ServerSideConnectionManager::CreateServerSide(communication_timeout);
}

void GameServer::RunIteration() {
  server_manager->HandleConnections();
  auto event = server_manager->PopAndGetOldestEvent();
  if (event != nullptr) {
    HandleEvent(event);
  }

  if (std::chrono::steady_clock::now() - last_world_sent >= send_world_interval) {
    last_world_sent = std::chrono::steady_clock::now();

    Change action(ChangeType::SET_WORLD);
    world->Serialize(action.payload);
    server_manager->Broadcast(action.payload);
  }
}

void GameServer::HandleEvent(const std::shared_ptr<communication::connection_layer::Event> &event) {
  switch (event->GetType()) {
    case communication::connection_layer::EventType::CONNECT: {
      auto spawn_position = world->GetSpawner().GenerateSpawnPosition();
      auto player = std::make_shared<Player>("?", spawn_position, event->GetPartnerId());

      world->AddPlayer(player);

      Change action(ChangeType::SET_OWN_ID);
      action.payload.push_back(event->GetPartnerId());
      server_manager->SendDataToConnection(event->GetPartnerId(), action.payload);

      break;
    }
    case communication::connection_layer::EventType::DISCONNECT: {
      assert(false);  // TODO: handle client disconnect
      break;
    }
    case communication::connection_layer::EventType::PAYLOAD: {
      Change change(std::dynamic_pointer_cast<communication::connection_layer::PayloadEvent>(event)->GetPayload());
      // The player ids are identical to the partner ids assigned by the ConnectionManager
      auto player_id = event->GetPartnerId();
      auto player = world->GetPlayerById(player_id);
      assert(player != nullptr);
      HandleChange(player, change);
      break;
    }
    default:
      throw std::runtime_error("Unexpected EventType");
  }
}

void GameServer::HandleChange(const std::shared_ptr<Player> &player, const Change &change) {
  switch (change.GetChangeType()) {
    case ChangeType::MOVE_UP: {
      MovePlayer(player, coordinate_distance_t(0, -1));
      break;
    }
    case ChangeType::MOVE_LEFT: {
      MovePlayer(player, coordinate_distance_t(-1, 0));
      break;
    }
    case ChangeType::MOVE_RIGHT: {
      MovePlayer(player, coordinate_distance_t(1, 0));
      break;
    }
    case ChangeType::MOVE_DOWN: {
      MovePlayer(player, coordinate_distance_t(0, 1));
      break;
    }
    default:
      throw std::runtime_error("Unexpected ChangeType");
  }
}

void GameServer::MovePlayer(const std::shared_ptr<Player> &player, const coordinate_distance_t &movement) {
  Position new_position = player->position + movement;

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

  if (new_position != player->position && !world->IsBlocked(new_position)) {
    player->MoveTo(new_position);
  }
}

const world::World &GameServer::GetWorld() const {
  return *world;
}
