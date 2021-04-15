#ifndef CROSSING_CONSOLES_TESTGAMEDEFINITION_H
#define CROSSING_CONSOLES_TESTGAMEDEFINITION_H

#include <gtest/gtest.h>

#include <utility>

#include "../../src/game/GameDefinition.h"

namespace game {

/**
 * \brief Overwrite `GameDefinition` constant `respawn_time` for faster testing.
 */
class TestGameDefinition : public GameDefinition {
 public:
  explicit TestGameDefinition(std::chrono::milliseconds respawn_time = std::chrono::milliseconds(5000))
      : GameDefinition(respawn_time){};
};

}  // namespace game
#endif  // CROSSING_CONSOLES_TESTGAMEDEFINITION_H
