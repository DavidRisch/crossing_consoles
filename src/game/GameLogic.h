#ifndef CROSSING_CONSOLES_GAMELOGIC_H
#define CROSSING_CONSOLES_GAMELOGIC_H

#include "GameDefinition.h"
#include "networking/Change.h"
#include "world/World.h"

namespace game {

/**
 * \brief Apply game logic to world and player on server and client side
 */
class GameLogic {
 public:
  /**
   * \brief Handle a `Change` produced by a `GameClient` controlling the specific player or by keyboard input.
   */
  static void HandleChange(world::Player &player, const networking::Change &change, world::World &world);

 private:
  /**
   * \brief Move the player with the given `movement`.
   */
  static void MovePlayer(world::Player &player, const common::coordinate_distance_t &movement, world::World &world);

  /**
   * \brief Return new position in world from given movement.
   */
  static common::Position PositionFromMovement(const common::Position &position,
                                               const common::coordinate_distance_t &movement, world::World &world);
};

}  // namespace game
#endif  // CROSSING_CONSOLES_GAMELOGIC_H
