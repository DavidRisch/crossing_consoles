#ifndef CROSSING_CONSOLES_PLAYER_LIST_H
#define CROSSING_CONSOLES_PLAYER_LIST_H

#include <functional>

#include "../common/Position.h"
#include "../world/World.h"
#include "ColoredCharMatrix.h"
#include "Table.h"

namespace game::visual {

/**
 * \brief A `TableRow` of `PlayerList`.
 */
class PlayerListRow : public TableRow {
 public:
  PlayerListRow(std::string name, common::Color player_color, int player_health, uint16_t player_score,
                double packet_loss_percentage, std::optional<std::chrono::microseconds> ping);

  const std::string player_name;
  const common::Color player_color;
  const int player_health;
  const uint16_t player_score;
  const double packet_loss_percentage;
  const std::optional<std::chrono::microseconds> ping;
};

/**
 * \brief Draws a list/table of `Player`s with information about each `Player`.
 */
class PlayerList : public Table<PlayerListRow> {
 public:
  explicit PlayerList(world::player_ptr_list_t &players);

  [[nodiscard]] std::list<PlayerListRow> MakeRows() const override;

 private:
  world::player_ptr_list_t &players;

  friend PlayerListRow;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_PLAYER_LIST_H
