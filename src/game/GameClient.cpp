#include "GameClient.h"

#include <chrono>
#include <thread>
#include <utility>

#include "../communication/connection_layer/event/PayloadEvent.h"
#include "networking/Change.h"
#include "terminal/ITerminal.h"
#include "world/WorldGenerator.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::terminal;
using namespace game::networking;
using namespace game::visual;

GameClient::GameClient(Player player, std::shared_ptr<ITerminal> terminal, const coordinate_size_t& world_size,
                       bool multiplayer)
    : player(std::move(player))
    , world(world_size)
    , terminal(std::move(terminal))
    , multiplayer(multiplayer) {
  this->world.AddPlayer(&this->player);
  coordinate_size_t viewport_size = Position(51, 25);
  compositor = std::make_unique<Compositor>(viewport_size, this->world, this->player);

  if (multiplayer) {
    client_manager = communication::connection_layer::ClientSideConnectionManager::CreateClientSide();
  } else {
    world = *WorldGenerator::GenerateWorld(world_size);
  }
}

void GameClient::Run() {
  while (keep_running) {
    ProcessInput();
    if (world.updated || player.updated) {
      terminal->SetScreen(compositor->CompositeViewport());
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }

    if (multiplayer) {
      client_manager->HandleConnections();

      auto event = client_manager->PopAndGetOldestEvent();
      if (event != nullptr) {
        if (event->GetType() == communication::connection_layer::EventType::PAYLOAD) {
          Change change(std::dynamic_pointer_cast<communication::connection_layer::PayloadEvent>(event)->GetPayload());
          if (change.GetChangeType() == ChangeType::SET_WORLD) {
            auto iterator = change.payload.begin();
            ++iterator;
            world.Update(World::Deserialize(iterator));
          } else {
            throw std::runtime_error("Unexpected ChangeType");
          }
        }
      }
    }
  }
}

void GameClient::ProcessInput() {
  coordinate_distance_t movement(0, 0);

  if (terminal->HasInput()) {
    int keypress = terminal->GetInput();
    switch (static_cast<KeyCode>(keypress)) {
      case KeyCode::W: {
        movement.Set(0, -1);
        if (multiplayer) {
          Change change(ChangeType::MOVE_UP);
          client_manager->SendDataToConnection(communication::ProtocolDefinition::server_partner_id, change.payload);
        }
        break;
      }
      case KeyCode::A: {
        movement.Set(-1, 0);
        if (multiplayer) {
          Change change(ChangeType::MOVE_LEFT);
          client_manager->SendDataToConnection(communication::ProtocolDefinition::server_partner_id, change.payload);
        }
        break;
      }
      case KeyCode::S: {
        movement.Set(0, 1);
        if (multiplayer) {
          Change change(ChangeType::MOVE_DOWN);

          client_manager->SendDataToConnection(communication::ProtocolDefinition::server_partner_id, change.payload);
        }
        break;
      }
      case KeyCode::D: {
        movement.Set(1, 0);
        if (multiplayer) {
          Change change(ChangeType::MOVE_RIGHT);

          client_manager->SendDataToConnection(communication::ProtocolDefinition::server_partner_id, change.payload);
        }
        break;
      }
      case KeyCode::ESCAPE:
        keep_running = false;
      default:
        return;
    }

    if (!multiplayer) {
      Position new_position = player.position + movement;

      if (new_position.x < 0) {
        new_position.x += world.size.x;
      } else if (new_position.x >= world.size.x) {
        new_position.x -= world.size.x;
      }
      if (new_position.y < 0) {
        new_position.y += world.size.y;
      } else if (new_position.y >= world.size.y) {
        new_position.y -= world.size.y;
      }

      if (new_position != player.position && !world.IsBlocked(new_position)) {
        player.MoveTo(new_position);
      }
    }
  }
}
