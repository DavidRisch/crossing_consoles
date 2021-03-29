#include "GameClient.h"

#include <cassert>
#include <chrono>
#include <thread>

#include "../communication/connection_layer/event/PayloadEvent.h"
#include "GameLogic.h"
#include "networking/Change.h"
#include "terminal/ITerminal.h"
#include "world/WorldGenerator.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::terminal;
using namespace game::networking;
using namespace game::visual;

GameClient::GameClient(const std::shared_ptr<Player>& player, const std::shared_ptr<ITerminal>& terminal,
                       const coordinate_size_t& world_size, bool multiplayer,
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
  } else {
    WorldGenerator world_generator;
    world = *world_generator.GenerateWorld(world_size);
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

      auto event = client_manager->PopAndGetOldestEvent();
      if (event != nullptr) {
        if (event->GetType() == communication::connection_layer::EventType::PAYLOAD) {
          Change change(std::dynamic_pointer_cast<communication::connection_layer::PayloadEvent>(event)->GetPayload());
          if (change.GetChangeType() == ChangeType::SET_WORLD) {
            auto iterator = change.payload.begin();
            ++iterator;
            auto server_world = World::Deserialize(iterator);
            world.Update(server_world);
          } else if (change.GetChangeType() == ChangeType::SET_OWN_ID) {
            player->player_id = change.payload.at(1);
          } else {
            throw std::runtime_error("Unexpected ChangeType");
          }
        }
      }
    }

    if (world.updated || player->updated) {
      terminal->SetScreen(compositor->CompositeViewport());
    }

    std::this_thread::sleep_for(std::chrono::microseconds(100));
  }
}

void GameClient::ProcessInput() {
  if (terminal->HasInput()) {
    int keypress = terminal->GetInput();
    auto keycode = static_cast<KeyCode>(keypress);
    auto change_type_it = map_key_to_change.find(keycode);

    if (change_type_it == map_key_to_change.end()) {
      if (keycode == KeyCode::ESCAPE) {
        keep_running = false;
      }
      return;
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
