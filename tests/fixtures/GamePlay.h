#ifndef CROSSING_CONSOLES_GAMEPLAY_H
#define CROSSING_CONSOLES_GAMEPLAY_H

#include <gtest/gtest.h>

#include <utility>

#include "../../src/game/GameClient.h"
#include "../../src/game/GameLogic.h"

namespace game {

class GamePlay : public ::testing::Test {
 public:
  GamePlay() = default;
  std::shared_ptr<world::World> world;
  std::shared_ptr<world::Player> player_first;
  std::shared_ptr<world::Player> player_second;

  static constexpr GameDefinition::Direction standard_direction = GameDefinition::Direction::EAST;

  // maps directions and positions in such a way, that one movement in the given direction leads to position (0,0)
  static const std::unordered_map<const GameDefinition::Direction, common::Position> map_direction_to_position;

  void initialize_game();

  void add_player();

  void spawn_projectile(int range = 10, int damage = 10, GameDefinition::Direction direction = standard_direction,
                        GameDefinition::player_id_t player_id = 1,
                        const common::Position& position = common::Position(0, 0));

  void move_player(std::shared_ptr<world::Player>& player, game::networking::ChangeType change, int steps);

  void projectile_collision(GameDefinition::Direction direction_first, GameDefinition::Direction direction_second);

  void reset_elements();
};

}  // namespace game
#endif  // CROSSING_CONSOLES_GAMEPLAY_H
