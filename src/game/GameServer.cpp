#include "GameServer.h"

#include <thread>

#include "../communication/connection_layer/event/PayloadEvent.h"
#include "GameLogic.h"
#include "networking/Change.h"
#include "networking/SerializationUtils.h"
#include "world/EmptyWorldGenerator.h"
#include "world/RandomWorldGenerator.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::networking;

GameServer::GameServer(const coordinate_size_t &world_size, bool empty_world,
                       communication::ProtocolDefinition::timeout_t communication_timeout) {
  if (empty_world) {
    EmptyWorldGenerator world_generator;
    world = world_generator.GenerateWorld(world_size);
  } else {
    RandomWorldGenerator world_generator;
    world = world_generator.GenerateWorld(world_size);
  }

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
  }

  if (std::chrono::steady_clock::now() - last_item_generated >= generate_item_interval) {
    last_item_generated = std::chrono::steady_clock::now();
    world->GetItemGenerator().GenerateItem();
  }

  if (std::chrono::steady_clock::now() - last_world_sent >= send_world_interval) {
    GameLogic::ReduceColoredFieldLifetimes(*world);

    last_world_sent = std::chrono::steady_clock::now();

    for (const auto &player : world->players) {
      // Check if player needs to be respawned
      GameLogic::HandlePlayerRespawn(*player, *world);

      // copy communication statistics into the player objects which are sent to clients
      const auto &connection_statistics = server_manager->GetStatisticsFromPartnerConnection(player->player_id);

      player->packet_loss_percentage = connection_statistics.CalculatePackageLoss().package_loss_percentage;
      player->ping = connection_statistics.GetAverageResponseTime();
    }

    Change action(ChangeType::UPDATE_WORLD);
    world->SerializeUpdate(action.payload);
    server_manager->Broadcast(action.payload);
  }
}

void GameServer::HandleEvent(const std::shared_ptr<communication::connection_layer::Event> &event) {
  switch (event->GetType()) {
    case communication::connection_layer::EventType::CONNECT: {
      if (world->players.size() >= max_player_count) {
        server_manager->CloseConnection(event->GetPartnerId());
        break;
      }

      auto spawn_position = world->GetSpawner().GenerateSpawnPosition();
      auto player = std::make_shared<Player>("?", Color::RED, spawn_position, GameDefinition::Direction::NORTH,
                                             event->GetPartnerId());

      world->AddPlayer(player);

      {
        Change action(ChangeType::SET_OWN_ID);
        action.payload.push_back(event->GetPartnerId());
        server_manager->SendDataToConnection(event->GetPartnerId(), action.payload);
      }
      {
        Change action(ChangeType::SET_WORLD);
        world->Serialize(action.payload);
        server_manager->SendDataToConnection(event->GetPartnerId(), action.payload);
      }

      break;
    }
    case communication::connection_layer::EventType::DISCONNECT: {
      auto player_id = event->GetPartnerId();

      auto player = world->GetPlayerById(player_id);
      if (player == nullptr) {
        break;  // client was never fully connected
      }

      world->RemovePlayer(player_id);
      break;
    }
    case communication::connection_layer::EventType::PAYLOAD: {
      Change change(std::dynamic_pointer_cast<communication::connection_layer::PayloadEvent>(event)->GetPayload());
      // The player ids are identical to the partner ids assigned by the ConnectionManager
      auto player_id = event->GetPartnerId();
      auto player = world->GetPlayerById(player_id);
      if (player == nullptr) {
        break;
      }
      switch (change.GetChangeType()) {
        case ChangeType::SET_NAME: {
          auto iterator = change.GetContentIterator();
          player->name = SerializationUtils::DeserializeString(iterator);
          break;
        }
        case ChangeType::SET_COLOR: {
          auto iterator = change.GetContentIterator();
          player->color = Color::Deserialize(iterator);
          break;
        }
        default:
          GameLogic::HandleChange(*player, change, *world);
          break;
      }

      break;
    }
    default:
      throw std::runtime_error("Unexpected EventType");
  }
}

const world::World &GameServer::GetWorld() const {
  return *world;
}
