#ifndef CROSSING_CONSOLES_COMPOSITOR_H
#define CROSSING_CONSOLES_COMPOSITOR_H

#include <memory>

#include "../common/Position.h"
#include "../world/Player.h"
#include "../world/World.h"
#include "ColoredCharMatrix.h"
#include "PlayerList.h"
#include "Renderer.h"
#include "StatisticsTable.h"

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
  bool show_statistics_table = false;
  bool simulate_bad_connection = false;

  Compositor(const common::coordinate_size_t& viewport_size, world::World& world, world::Player& player);

  /**
   * \brief Create game output that is printed to the console.
   */
  [[nodiscard]] const ColoredCharMatrix& CompositeViewport();

  void SetConnectionStatistics(const communication::connection_layer::ConnectionStatistics& connection_statistics);

 private:
  std::unique_ptr<PlayerList> player_list;
  std::unique_ptr<StatisticsTable> statistics_table;

  /**
   * \brief Composite header containing game title, and game information.
   */
  [[nodiscard]] ColoredCharMatrix CompositeHeader(int viewport_width) const;

  /**
   * \brief Composite trailer, shown below the game output.
   */
  [[nodiscard]] ColoredCharMatrix CompositeTrailer(int viewport_width, bool has_item);

  static ColoredCharMatrix GenerateSeparatorLine(int viewport_size, bool is_first, bool is_last);

  static void SetBorderLines(ColoredCharMatrix& character_matrix, int position_y);

  /**
   * \brief Add a frame around the given `ColoredCharMatrix`.
   */
  static ColoredCharMatrix DrawFrame(const ColoredCharMatrix& content);

  /// border size = number of characters used to draw the frame
  static const int border_size_x = 1;

  static const int header_height = 6;
  static const int trailer_height = 3;

  /// calculate number of characters needed to display world output
  const common::coordinate_size_t game_character_count = game_viewport_size * block_size;

  const int game_output_size_x =
      game_character_count.x + border_size_x * 2;  // one character at the left and right is added for box lines
  const int game_output_size_y = game_character_count.y + header_height + trailer_height;

  /// This is an attribute because clearing this object from scratch is slow.
  ColoredCharMatrix game_output;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_COMPOSITOR_H
