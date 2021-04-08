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

  /// Damage and range of `Gun`
  static constexpr int gun_damage = 1;
  static constexpr uint8_t gun_range = 10;

  /// Damage of `Sword`
  static constexpr int sword_damage = 2;

  /// Healing of `Heart`
  static constexpr int heart_healing = 1;

  /// Value of `Points`
  static constexpr uint8_t points_value = 10;
};

}  // namespace game

#endif  // CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
