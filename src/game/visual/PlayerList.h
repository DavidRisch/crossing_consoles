#ifndef CROSSING_CONSOLES_PLAYER_LIST_H
#define CROSSING_CONSOLES_PLAYER_LIST_H

#include <functional>

#include "../world/World.h"
#include "ColoredCharMatrix.h"

namespace game::visual {

/**
 * \brief Draws a list/table of `Player`s with information about each `Player`.
 */
class PlayerList {
 public:
  explicit PlayerList(world::player_ptr_list_t &players);

  /**
   * \brief Render the table including its border.
   */
  [[nodiscard]] game::visual::ColoredCharMatrix Render() const;

  /**
   * \brief Calculate the width in chars of all columns and dividers/borders.
   */
  [[nodiscard]] static int CalculateTotalWidth();

 private:
  [[nodiscard]] static game::visual::ColoredCharMatrix MakeHeader();
  [[nodiscard]] static game::visual::ColoredCharMatrix MakeDivider(bool is_first, bool is_last);

  world::player_ptr_list_t &players;

  class Row {
   public:
    Row(std::string name, common::Color player_color, int player_health, uint16_t player_score,
        double packet_loss_percentage, std::optional<std::chrono::microseconds> ping);

    /**
     * \brief Render a complete row of cells.
     */
    [[nodiscard]] game::visual::ColoredCharMatrix RenderRow(const PlayerList &p) const;

    const std::string player_name;
    const common::Color player_color;
    const int player_health;
    const uint16_t player_score;
    const double packet_loss_percentage;
    const std::optional<std::chrono::microseconds> ping;
  };

  class Column {
   public:
    typedef std::function<void(const Row &row, game::visual::ColoredCharMatrix &field)> render_lambda_t;

    Column(int width, std::string title, render_lambda_t render_lambda);

    /**
     * \brief Render a single cell.
     */
    [[nodiscard]] game::visual::ColoredCharMatrix RenderCell(const Row &row) const;

    /**
     * \brief Render a single header cell.
     */
    [[nodiscard]] game::visual::ColoredCharMatrix RenderTitle() const;

    [[nodiscard]] int GetWidth() const;

   private:
    const int width;
    const std::string title;
    const render_lambda_t render_lambda;
  };

  /// Defines the layout of the table.
  static const std::list<Column> columns;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_PLAYER_LIST_H
