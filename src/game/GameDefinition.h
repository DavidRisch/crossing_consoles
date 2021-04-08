#ifndef CROSSING_CONSOLES_GAME_DEFINITION_H
#define CROSSING_CONSOLES_GAME_DEFINITION_H

#include <time.h>

#include <chrono>
#include <cstdint>

namespace game {

/**
 * \brief Defines constants used in the game.
 */
class GameDefinition {
 public:
  /// Moving direction of `Player` and `Projectile` in `World`.
  enum Direction : char { NORTH, EAST, SOUTH, WEST };

  /// Set waiting duration until player respawn
  static constexpr std::chrono::duration<int64_t, std::milli> respawn_time = std::chrono::milliseconds(5000);

  /// Used to reference players.
  typedef uint16_t player_id_t;

  /// Maximum number of items in the world at the same time
  static constexpr int max_items_in_world = 3;
};

}  // namespace game

#endif  // CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
