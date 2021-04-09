#ifndef CROSSING_CONSOLES_ITEMS_H
#define CROSSING_CONSOLES_ITEMS_H

#include <gtest/gtest.h>

#include "../../src/game/GameClient.h"
#include "../../src/game/GameLogic.h"
#include "../../src/game/world/WorldGenerator.h"

namespace game {

class Items : public ::testing::Test {
 public:
  std::shared_ptr<world::World> world;
  std::shared_ptr<world::Player> player_first;
  std::shared_ptr<world::Player> player_second;

  Items() = default;

  void generate_world() {
    reset_elements();
    world = world::WorldGenerator::GenerateWorld(common::coordinate_size_t(20, 15));
  }

  void add_one_player() {
    player_first = std::make_shared<world::Player>("player1", common::Position(0, 0));
    player_first->player_id = 1;
    player_first->direction = GameDefinition::Direction::EAST;

    world->AddPlayer(player_first);
  }

  void add_two_players() {
    player_first = std::make_shared<world::Player>("player1", common::Position(0, 0));
    player_first->player_id = 1;
    player_first->direction = GameDefinition::Direction::EAST;

    player_second = std::make_shared<world::Player>("player2", common::Position(2, 0));
    player_second->player_id = 2;

    world->AddPlayer(player_first);
    world->AddPlayer(player_second);
  }

  void move_player(std::shared_ptr<world::Player>& player, game::networking::ChangeType change, int steps) {
    for (int i = 0; i < steps; i++) {
      game::networking::Change movement_change(change);
      GameLogic::HandleChange(*player, movement_change, *world);
    }
  }

  void reset_elements() {
    world.reset();
    player_first.reset();
    player_second.reset();
  }
};
}  // namespace game
#endif  // CROSSING_CONSOLES_ITEMS_H
