#ifndef CROSSING_CONSOLES_GAMECLIENT_H
#define CROSSING_CONSOLES_GAMECLIENT_H

#include <memory>
#include <unordered_map>

#include "../communication/connection_layer/connection/ClientSideConnectionManager.h"
#include "networking/Change.h"
#include "terminal/ITerminal.h"
#include "visual/Compositor.h"
#include "world/Player.h"
#include "world/World.h"

namespace game {

enum class KeyCode { ESCAPE = 27, W = 'w', A = 'a', S = 's', D = 'd', SPACE = ' ', Y = 'y', X = 'x' };

/**
 * \brief Handles keyboard input and connection to `GameServer`.
 */
class GameClient {
 public:
  GameClient(
      const std::shared_ptr<world::Player>& player, const std::shared_ptr<terminal::ITerminal>& terminal,
      const common::coordinate_size_t& world_size, bool multiplayer = false, bool empty_world = false,
      communication::ProtocolDefinition::timeout_t communication_timeout = communication::ProtocolDefinition::timeout,
      GameDefinition game_definition = GameDefinition());

  /**
   * \brief Update world, player, projectiles and process changes.
   * \details In multiplayer mode, events from 'ConnectionManager' are handled additionally.
   */
  void Run();

  /**
   * \brief Start the process of exiting the game. `Run()` will stop some time after this method is called.
   */
  void StartExit();

  /**
   * \brief Handle keyboard input.
   * \details In single player mode, changes are applied to the world. In multiplayer mode, changes are sent to
   * `GameServer`.
   */
  void ProcessInput();

  const world::World& GetWorld() const;

 private:
  std::weak_ptr<world::Player> weak_player;
  world::World world;
  std::shared_ptr<terminal::ITerminal> terminal;
  std::unique_ptr<visual::Compositor> compositor;
  std::shared_ptr<communication::connection_layer::ClientSideConnectionManager> client_manager;
  bool keep_running = true;
  bool multiplayer;

  /// Holds all game definitions valid for the game in SinglePlayer mode.
  const GameDefinition game_definition;

  /// True if the own `Player` is definitely represented on the server.
  bool server_initialised = false;

  /// Indicates some kind of change which requires a new frame to be drawn.
  bool updated = false;

  /// Map keycode to associated change
  std::unordered_map<const KeyCode, networking::Change> map_key_to_change = {
      {KeyCode::W, networking::Change(networking::ChangeType::MOVE_UP)},
      {KeyCode::S, networking::Change(networking::ChangeType::MOVE_DOWN)},
      {KeyCode::A, networking::Change(networking::ChangeType::MOVE_LEFT)},
      {KeyCode::D, networking::Change(networking::ChangeType::MOVE_RIGHT)},
      {KeyCode::SPACE, networking::Change(networking::ChangeType::USE_ITEM)},
  };

  std::chrono::time_point<std::chrono::steady_clock> last_draw;
  static constexpr auto min_draw_interval = std::chrono::milliseconds(50);

  /**
   * \brief Handle an `Event` caused by the `GameServer`.
   */
  void HandleEvent(const std::shared_ptr<world::Player>& player,
                   const std::shared_ptr<communication::connection_layer::Event>& event);
};

}  // namespace game

#endif  // CROSSING_CONSOLES_GAMECLIENT_H
