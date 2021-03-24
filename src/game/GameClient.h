#ifndef CROSSING_CONSOLES_GAMECLIENT_H
#define CROSSING_CONSOLES_GAMECLIENT_H

#include <memory>

#include "../communication/connection_layer/connection/ClientSideConnectionManager.h"
#include "networking/Change.h"
#include "terminal/ITerminal.h"
#include "visual/Compositor.h"
#include "world/Player.h"
#include "world/World.h"

namespace game {

enum class KeyCode { ESCAPE = 27, W = 'w', A = 'a', S = 's', D = 'd', SPACE = ' ' };

class GameClient {
 public:
  GameClient(
      const std::shared_ptr<world::Player>& player, const std::shared_ptr<terminal::ITerminal>& terminal,
      const common::coordinate_size_t& world_size, bool multiplayer = false,
      communication::ProtocolDefinition::timeout_t communication_timeout = communication::ProtocolDefinition::timeout);

  /**
   * \brief Update world, player and process changes.
   * \details In multiplayer node, events from 'ConnectionManager' are handled additionally.
   */
  void Run();

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
};

}  // namespace game

#endif  // CROSSING_CONSOLES_GAMECLIENT_H
