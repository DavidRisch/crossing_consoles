#ifndef CROSSING_CONSOLES_HEALTH_DISPLAY_H
#define CROSSING_CONSOLES_HEALTH_DISPLAY_H

#include "../world/Player.h"
#include "ColoredCharMatrix.h"

namespace game::visual {

/**
 * \brief Used to generate a visual representation of a players health.
 */
class HealthDisplay {
 public:
  /**
   * \brief Creates a row of hearts displaying the given health value.
   */
  [[nodiscard]] static ColoredCharMatrix Render(int health);

  /// Width of a single heart.
  static const int heart_width = 2;
  /// Width of a rendered row.
  static constexpr int width = heart_width * world::Player::max_health;
};
}  // namespace game::visual

#endif  // CROSSING_CONSOLES_HEALTH_DISPLAY_H
