#include "GameServer.h"

#include <cassert>
#include <thread>

#include "../communication/connection_layer/event/PayloadEvent.h"
#include "GameLogic.h"
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
  while (event != nullptr) {
    HandleEvent(event);
    event = server_manager->PopAndGetOldestEvent();
  }

  if (std::chrono::steady_clock::now() - last_moving_projectiles_updated >= update_projectiles_interval) {
    // Moving projectiles should be updated in a lower frequency
    last_moving_projectiles_updated = std::chrono::steady_clock::now();
    GameLogic::HandleProjectiles(*world);

    // TODO Set score
    // TODO Remove dead players!
  }

  if (std::chrono::steady_clock::now() - last_world_sent >= send_world_interval) {
    last_world_sent = std::chrono::steady_clock::now();

    // copy communication statistics into the player objects which are sent to clients
    for (const auto &player : world->players) {
      const auto &connection_statistics = server_manager->GetStatisticsFromPartnerConnection(player->player_id);

      player->packet_loss_percentage = connection_statistics.CalculatePackageLoss().package_loss_percentage;
      player->ping = connection_statistics.CalculateAverageResponseTime();
    }

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
      auto player_id = event->GetPartnerId();
      world->RemovePlayer(player_id);
      break;
    }
    case communication::connection_layer::EventType::PAYLOAD: {
      Change change(std::dynamic_pointer_cast<communication::connection_layer::PayloadEvent>(event)->GetPayload());
      // The player ids are identical to the partner ids assigned by the ConnectionManager
      auto player_id = event->GetPartnerId();
      auto player = world->GetPlayerById(player_id);
      assert(player != nullptr);
      GameLogic::HandleChange(*player, change, *world);
      break;
    }
    default:
      throw std::runtime_error("Unexpected EventType");
  }
}

const world::World &GameServer::GetWorld() const {
  return *world;
}
