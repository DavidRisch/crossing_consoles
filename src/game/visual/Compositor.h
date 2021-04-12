#ifndef CROSSING_CONSOLES_COMPOSITOR_H
#define CROSSING_CONSOLES_COMPOSITOR_H

#include <memory>

#include "../common/Position.h"
#include "../world/Player.h"
#include "../world/World.h"
#include "ColoredCharMatrix.h"
#include "PlayerList.h"
#include "Renderer.h"

namespace game::visual {

/**
 * \brief Generates a `ColoredCharMatrix` from the `World`, `Wall`s and `Player`s.
 */
class Compositor {
 public:
  const common::coordinate_size_t block_size = common::coordinate_size_t(3, 3);
  common::coordinate_size_t game_viewport_size;
  std::unique_ptr<Renderer> renderer;
  world::World* world;
  world::Player* player;
  bool show_player_list = false;

  Compositor(const common::coordinate_size_t& viewport_size, world::World& world, world::Player& player);

  /**
   * \brief Create game output that is printed to the console.
   */
  [[nodiscard]] ColoredCharMatrix CompositeViewport() const;

 private:
  std::unique_ptr<PlayerList> player_list;

  /**
   * \brief Composite header containing game title, and game information.
   */
  [[nodiscard]] ColoredCharMatrix CompositeHeader(int viewport_width) const;

  /**
   * \brief Composite trailer, shown below the game output.
   */
  [[nodiscard]] static ColoredCharMatrix CompositeTrailer(int viewport_width);

  static ColoredCharMatrix GenerateSeparatorLine(int viewport_size, bool is_first, bool is_last);

  static void SetBorderLines(ColoredCharMatrix& character_matrix, int position_y);
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COMPOSITOR_H
