#ifndef CROSSING_CONSOLES_GAMESERVER_H
#define CROSSING_CONSOLES_GAMESERVER_H

#include <memory>

#include "../communication/connection_layer/connection/ServerSideConnectionManager.h"
#include "networking/Change.h"
#include "world/World.h"

namespace game {

/**
 * Server to handle game logic and connections to `GameClient`.
 */
class GameServer {
 public:
  explicit GameServer(const common::coordinate_size_t &world_size);

  /**
   * \brief Handle connections to `GameClient`s. Should be called in a loop.
   */
  void RunIteration();

 private:
  /**
   * \brief Handle a `Event` caused by a `GameClient`.
   */
  void HandleEvent(const std::shared_ptr<communication::connection_layer::Event> &event);

  /**
   * \brief Handle a `Change` sent by a `GameClient`.
   */
  void HandleChange(const networking::Change &change);

  /**
   * Move the player with the given `movement`.
   */
  void MovePlayer(const common::coordinate_distance_t &movement);

  std::shared_ptr<world::World> world;

  std::shared_ptr<communication::connection_layer::ServerSideConnectionManager> server_manager;

  /// Used to send world updates at a constant frequency.
  std::chrono::time_point<std::chrono::steady_clock> last_world_sent;
  static constexpr auto send_world_interval = std::chrono::milliseconds(100);
};

}  // namespace game

#endif  // CROSSING_CONSOLES_GAMESERVER_H
