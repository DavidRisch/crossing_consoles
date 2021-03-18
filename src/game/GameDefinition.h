#ifndef CROSSING_CONSOLES_GAME_DEFINITION_H
#define CROSSING_CONSOLES_GAME_DEFINITION_H

#include <cstdint>

namespace game {

/**
 * \brief Defines constants used in the game.
 */
class GameDefinition {
 public:
  /// Used to reference players.
  typedef uint16_t player_id_t;
};

}  // namespace game

#endif  // CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
