#include "GameClient.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

#include "../communication/byte_layer/connection_simulator/ConnectionSimulatorFlaky.h"
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
                       communication::ProtocolDefinition::timeout_t communication_timeout,
                       GameDefinition game_definition, bool instant_input)
    : weak_player(player)
    , world(world_size)
    , terminal(terminal)
    , multiplayer(multiplayer)
    , game_definition(std::move(game_definition))
    , instant_input(instant_input) {
  assert(player != nullptr);
  assert(terminal != nullptr);

  coordinate_size_t viewport_size = Position(51, 18);
  compositor = std::make_unique<Compositor>(viewport_size, world_render_copy, *player);

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
  if (multiplayer) {
    StartCommunicationThread();

    try {
      RunMainThread();
      communication_thread.join();
    } catch (const std::exception& e) {
      if (multiplayer) {
        // make sure the communication_thread is joined. Otherwise it can hide the real exception.
        communication_thread.join();
      }
      throw e;
    }

    client_manager->CloseConnectionWithServer();
    // wait until connection is properly closed
    while (client_manager->HasConnections()) {
      client_manager->HandleConnections();
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  } else {
    RunMainThread();
  }
}

void GameClient::RunMainThread() {
  while (keep_running) {
    auto player = weak_player.lock();
    assert(player != nullptr);

    auto now = std::chrono::steady_clock::now();

    auto optional_change = ProcessInput(now);

    if (optional_change.has_value()) {
      HandleOwnChange(*optional_change, now);
    }

    if (world.updated || player->updated || updated) {
      if (now - last_draw >= min_draw_interval) {
        last_draw = now;

        if (!multiplayer) {
          GameLogic::HandleProjectiles(world);
          GameLogic::HandlePlayerRespawn(*player, world, game_definition.respawn_time);
        }

        world.updated = false;
        player->updated = false;
        updated = false;

        {
          std::lock_guard<std::mutex> lock_guard(world_mutex);
          world_render_copy = world;
        }

        terminal->SetScreen(compositor->CompositeViewport());
      }
    }

    const auto& start = now;
    auto end = std::chrono::steady_clock::now();
    auto delta = end - start;
    if (delta < min_main_loop_interval) {
      std::this_thread::sleep_for(min_main_loop_interval - delta);
    }
  }
}

std::optional<Change> GameClient::ProcessInput(std::chrono::steady_clock::time_point now) {
  if (terminal->HasInput()) {
    int keypress = terminal->GetInput();
    auto keycode = static_cast<KeyCode>(keypress);

    if (keycode == KeyCode::SHELL_ESCAPE) {
      if (!terminal->HasInput()) {
        // The literal escape key was pressed
        StartExit();
        return {};
      }
      // Some kind of escape sequence was entered, might be an arrow key

#ifndef _WIN32
      // https://stackoverflow.com/a/11432632/13623303
      if (terminal->GetInput() != '[') {
        return {};
      }

      if (!terminal->HasInput()) {
        return {};
      }

      int value = terminal->GetInput();
      switch (value) {
        case 'A':
          keycode = KeyCode::USE_UP;
          break;
        case 'B':
          keycode = KeyCode::USE_DOWN;
          break;
        case 'C':
          keycode = KeyCode::USE_RIGHT;
          break;
        case 'D':
          keycode = KeyCode::USE_LEFT;
          break;
        default:
          return {};
      }

#endif
    }

#ifdef _WIN32
    // https://stackoverflow.com/a/10473315/13623303
    // check if escape sequence for an arrow key was entered
    if (keypress == 0 || keypress == 224) {
      int value = terminal->GetInput();
      switch (value) {
        case 72:
          keycode = KeyCode::USE_UP;
          break;
        case 80:
          keycode = KeyCode::USE_DOWN;
          break;
        case 77:
          keycode = KeyCode::USE_RIGHT;
          break;
        case 75:
          keycode = KeyCode::USE_LEFT;
          break;
        default:
          return {};
      }
    }
#endif

    auto change_type_it = map_key_to_change.find(keycode);

    if (change_type_it == map_key_to_change.end()) {
      switch (keycode) {
        case KeyCode::Y: {
          compositor->show_player_list ^= true;
          if (compositor->show_player_list) {
            compositor->show_statistics_table = false;
          }
          updated = true;
          return {};
        }
        case KeyCode::X: {
          compositor->show_statistics_table ^= true;
          if (compositor->show_statistics_table) {
            compositor->show_player_list = false;
          }
          updated = true;
          return {};
        }
        case KeyCode::C: {
          simulate_bad_connection ^= true;
          compositor->simulate_bad_connection = simulate_bad_connection;

          std::shared_ptr<communication::byte_layer::IConnectionSimulator> connection_simulator;
          if (simulate_bad_connection) {
            connection_simulator = std::make_shared<communication::byte_layer::ConnectionSimulatorFlaky>(
                communication::byte_layer::ConnectionSimulatorFlaky::Parameters(0, 100, 5));
          } else {
            connection_simulator = std::make_shared<communication::byte_layer::ConnectionSimulatorPerfect>();
          }

          auto socket_byte_stream = client_manager->GetSocketByteStream();
          assert(socket_byte_stream != nullptr);
          socket_byte_stream->SetConnectionSimulatorOutgoing(connection_simulator);
          return {};
        }
        default:
          return {};
      }
    }

    auto& change = change_type_it->second;

    if (!instant_input) {
      if (change.IsMovement()) {
        if (last_move + min_move_interval < now) {
          last_move = now;
        } else {
          next_move_type = change.GetChangeType();
          return {};
        }
      }

      if (change.IsItemUsage()) {
        if (last_item_usage + min_item_usage_interval < now) {
          last_item_usage = now;
        } else {
          next_item_usage_type = change.GetChangeType();
          return {};
        }
      }
    }

    return {change};
  } else {
    // if no key is pressed, handle old keys. This makes a double key press possible, the second key is handled here.

    if (next_move_type.has_value()) {
      if (last_move + min_move_interval < now) {
        next_move_type.reset();
        return networking::Change(*next_move_type);
      }
    }

    if (next_item_usage_type.has_value()) {
      if (last_item_usage + min_item_usage_interval < now) {
        next_item_usage_type.reset();
        return networking::Change(*next_item_usage_type);
      }
    }
  }

  return {};
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

void GameClient::HandleOwnChange(const networking::Change& change, std::chrono::steady_clock::time_point now) {
  if (change.IsMovement()) {
    last_move = now;
  }

  if (change.IsItemUsage()) {
    last_item_usage = now;
  }

  if (multiplayer) {
    std::lock_guard<std::mutex> lock_guard(communication_mutex);
    client_manager->SendDataToServer(change.payload);
  } else {
    auto player = weak_player.lock();
    assert(player != nullptr);
    GameLogic::HandleChange(*player, change, world);
  }
}

void GameClient::StartCommunicationThread() {
  communication_thread = std::thread([this] {
    while (keep_running) {
      auto start = std::chrono::steady_clock::now();

      {
        std::lock_guard<std::mutex> lock_guard(communication_mutex);
        client_manager->HandleConnections(start);
      }

      {
        std::lock_guard<std::mutex> lock_guard(world_mutex);

        auto player = weak_player.lock();
        assert(player != nullptr);
        HandleEvent(player, client_manager->PopAndGetOldestEvent());
      }

      auto end = std::chrono::steady_clock::now();
      auto delta = end - start;
      if (delta < min_communication_loop_interval) {
        std::this_thread::sleep_for(min_communication_loop_interval - delta);
      }
    }
  });
}
