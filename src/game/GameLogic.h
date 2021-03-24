#ifndef CROSSING_CONSOLES_GAMELOGIC_H
#define CROSSING_CONSOLES_GAMELOGIC_H

#include "networking/Change.h"
#include "world/World.h"

namespace game {
class GameLogic {
  /**
   * Apply game logic to world and player on server and client side
   */

 public:
  /**
   * Move the player with the given `movement`.
   */
  static void MovePlayer(const std::shared_ptr<world::Player> &player, const common::coordinate_distance_t &movement,
                         const std::shared_ptr<world::World> &world);

  /**
   * \brief Handle a `Change` sent by a `GameClient` or received by `GameServer` controlling the specific player.
   */
  static void HandleChange(const std::shared_ptr<world::Player> &player, const networking::Change &change,
                           const std::shared_ptr<world::World> &world);
};

}  // namespace game
#endif  // CROSSING_CONSOLES_GAMELOGIC_H
