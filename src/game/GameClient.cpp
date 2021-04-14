#include "GameClient.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

#include "../communication/connection_layer/event/PayloadEvent.h"
#include "GameLogic.h"
#include "networking/Change.h"
#include "networking/SerializationUtils.h"
#include "terminal/ITerminal.h"
#include "world/EmptyWorldGenerator.h"
#include "world/RandomWorldGenerator.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::terminal;
using namespace game::networking;
using namespace game::visual;

GameClient::GameClient(const std::shared_ptr<Player>& player, const std::shared_ptr<ITerminal>& terminal,
                       const coordinate_size_t& world_size, bool multiplayer, bool empty_world,
                       communication::ProtocolDefinition::timeout_t communication_timeout)
    : weak_player(player)
    , world(world_size)
    , terminal(terminal)
    , multiplayer(multiplayer) {
  assert(player != nullptr);
  assert(terminal != nullptr);

  coordinate_size_t viewport_size = Position(51, 25);
  compositor = std::make_unique<Compositor>(viewport_size, world, *player);

  if (multiplayer) {
    client_manager =
        communication::connection_layer::ClientSideConnectionManager::CreateClientSide(communication_timeout);

    compositor->SetConnectionStatistics(
        client_manager->GetStatisticsFromPartnerConnection(communication::ProtocolDefinition::server_partner_id));

    {
      Change change(ChangeType::SET_NAME);
      SerializationUtils::SerializeString(player->name, change.payload);
      client_manager->SendDataToServer(change.payload);
    }
    {
      Change change(ChangeType::SET_COLOR);
      player->color.Serialize(change.payload);
      client_manager->SendDataToServer(change.payload);
    }
  } else {
    if (empty_world) {
      EmptyWorldGenerator world_generator;
      world = *world_generator.GenerateWorld(world_size);
    } else {
      RandomWorldGenerator world_generator;
      world = *world_generator.GenerateWorld(world_size);
    }
  }

  world.AddPlayer(player);
}

void GameClient::Run() {
  while (keep_running) {
    auto player = weak_player.lock();
    assert(player != nullptr);

    ProcessInput();

    if (multiplayer) {
      client_manager->HandleConnections();
      HandleEvent(player, client_manager->PopAndGetOldestEvent());
    }

    if (world.updated || player->updated || updated) {
      if (!multiplayer) {
        GameLogic::HandleProjectiles(world);
        GameLogic::HandlePlayerRespawn(*player, world);
      }

      updated = false;
      terminal->SetScreen(compositor->CompositeViewport());
    }

    std::this_thread::sleep_for(std::chrono::microseconds(100));
  }

  if (multiplayer) {
    client_manager->CloseConnectionWithServer();
    // wait until connection is properly closed
    while (client_manager->HasConnections()) {
      client_manager->HandleConnections();
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  }
}

void GameClient::ProcessInput() {
  if (terminal->HasInput()) {
    int keypress = terminal->GetInput();
    auto keycode = static_cast<KeyCode>(keypress);
    auto change_type_it = map_key_to_change.find(keycode);

    if (change_type_it == map_key_to_change.end()) {
      switch (keycode) {
        case KeyCode::ESCAPE: {
          StartExit();
          return;
        }
        case KeyCode::Y: {
          compositor->show_player_list ^= true;
          updated = true;
          return;
        }
        case KeyCode::X: {
          compositor->show_statistics_table ^= true;
          updated = true;
          return;
        }
        default:
          return;
      }
    }

    auto change = change_type_it->second;
    if (multiplayer) {
      client_manager->SendDataToServer(change.payload);
    } else {
      auto player = weak_player.lock();
      assert(player != nullptr);
      GameLogic::HandleChange(*player, change, world);
    }
  }
}

const world::World& GameClient::GetWorld() const {
  return world;
}

void GameClient::StartExit() {
  keep_running = false;
}

void GameClient::HandleEvent(const std::shared_ptr<Player>& player,
                             const std::shared_ptr<communication::connection_layer::Event>& event) {
  if (event == nullptr) {
    return;
  }
  if (event->GetType() != communication::connection_layer::EventType::PAYLOAD) {
    if (event->GetType() == communication::connection_layer::EventType::DISCONNECT) {
      std::cout << "Server disconnected. Maximum player count reached." << std::endl;
      exit(1);
    }
    return;
  }

  Change change(std::dynamic_pointer_cast<communication::connection_layer::PayloadEvent>(event)->GetPayload());
  switch (change.GetChangeType()) {
    case (ChangeType::SET_WORLD): {
      if (server_initialised) {
        auto iterator = change.GetContentIterator();
        auto server_world = World::Deserialize(iterator);

        world.UpdateWalls(server_world);
        world.UpdateWithoutWalls(server_world);

        assert(world.GetPlayerById(player->player_id) != nullptr);  // the own player should never be removed
      }
      break;
    }
    case (ChangeType::UPDATE_WORLD): {
      if (server_initialised) {
        auto iterator = change.GetContentIterator();
        World server_world(world.size);
        World::DeserializeUpdate(iterator, server_world);

        world.UpdateWithoutWalls(server_world);

        assert(world.GetPlayerById(player->player_id) != nullptr);  // the own player should never be removed
      }
      break;
    }
    case (ChangeType::SET_OWN_ID): {
      player->player_id = change.payload.at(1);
      server_initialised = true;
      break;
    }
    default: {
      throw std::runtime_error("Unexpected ChangeType");
    }
  }
}
