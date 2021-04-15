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
  GameDefinition(std::chrono::milliseconds respawn_time = std::chrono::milliseconds(5000))
      : respawn_time(respawn_time){};

  /// Moving direction of `Player` and `Projectile` in `World`.
  enum Direction : char { NORTH, EAST, SOUTH, WEST };

  /// Set waiting duration until player respawn
  const std::chrono::duration<int64_t, std::milli> respawn_time;

  /// Used to reference players.
  typedef uint16_t player_id_t;

  /// Maximum number of items in the world at the same time
  static constexpr int max_items_in_world = 10;

  /// Minimum length of a `Player`'s name
  static const int name_length_min = 2;
  /// Maximum length of a `Player`'s name
  static const int name_length_max = 16;

  /// Lifetime of the `ColoredField`s representing damage caused on a field
  static const uint8_t colored_field_lifetime = 5;
};

}  // namespace game

#endif  // CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
