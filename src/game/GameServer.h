#ifndef CROSSING_CONSOLES_GAMESERVER_H
#define CROSSING_CONSOLES_GAMESERVER_H

#include <memory>

#include "../communication/connection_layer/connection/ServerSideConnectionManager.h"
#include "World.h"
#include "networking/Change.h"

/**
 * Server to handle game logic and connections to `GameClient`.
 */
class GameServer {
 public:
  explicit GameServer(const coordinate_size_t &world_size);

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
  void HandleChange(const Change &change);

  /**
   * Move the player with the given `movement`.
   */
  void MovePlayer(const coordinate_distance_t &movement);

  std::shared_ptr<World> world;

  std::shared_ptr<communication::connection_layer::ServerSideConnectionManager> server_manager;

  /// Used to send world updates at a constant frequency.
  std::chrono::time_point<std::chrono::steady_clock> last_world_sent;
  static constexpr auto send_world_interval = std::chrono::milliseconds(100);
};

#endif  // CROSSING_CONSOLES_GAMESERVER_H
