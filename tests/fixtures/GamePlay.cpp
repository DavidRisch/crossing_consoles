#include "GamePlay.h"

#include "../../src/game/world/EmptyWorldGenerator.h"

using namespace game;
using namespace game::common;
using namespace game::world;

const std::unordered_map<const GameDefinition::Direction, common::Position> GamePlay::map_direction_to_position = {
    {GameDefinition::NORTH, Position(0, 1)},
    {GameDefinition::SOUTH, Position(0, -1)},
    {GameDefinition::WEST, Position(1, 0)},
    {GameDefinition::EAST, Position(-1, 0)},

};

void GamePlay::initialize_game() {
  reset_elements();
  EmptyWorldGenerator world_generator;
  world = world_generator.GenerateWorld(common::coordinate_size_t(21, 21));
}

void GamePlay::add_player() {
  player_first = std::make_shared<world::Player>("player1", common::Position(0, 5));
  player_first->player_id = 1;
  player_first->direction = GameDefinition::Direction::NORTH;

  player_second = std::make_shared<world::Player>("player2", common::Position(5, 0));
  player_second->player_id = 2;

  world->AddPlayer(player_first);
  world->AddPlayer(player_second);
}

void GamePlay::spawn_projectile(int range, int damage, GameDefinition::Direction direction,
                                GameDefinition::player_id_t player_id, const common::Position& position) {
  world->AddProjectile(std::make_shared<Projectile>(Projectile(range, damage, player_id, position, direction)));
}

void GamePlay::move_player(std::shared_ptr<world::Player>& player, game::networking::ChangeType change, int steps) {
  for (int i = 0; i < steps; i++) {
    game::networking::Change movement_change(change);
    GameLogic::HandleChange(*player, movement_change, *world);
  }
}

void GamePlay::projectile_collision(GameDefinition::Direction direction_first,
                                    GameDefinition::Direction direction_second) {
  spawn_projectile(10, 1, direction_first, 1, map_direction_to_position.find(direction_first)->second);
  spawn_projectile(1, 1, direction_second, 2, map_direction_to_position.find(direction_second)->second);
  GameLogic::HandleProjectiles(*world);
  EXPECT_TRUE(world->GetProjectiles().empty());
}

void GamePlay::reset_elements() {
  world.reset();
  player_first.reset();
  player_second.reset();
}
