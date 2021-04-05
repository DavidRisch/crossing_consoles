#include "GameLogic.h"

#include <algorithm>
#include <cassert>

#include "world/Projectile.h"
#include "world/items/Weapon.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::networking;

const std::unordered_map<const GameDefinition::Direction, common::coordinate_distance_t>
    GameLogic::map_direction_to_movement = {
        {GameDefinition::NORTH, common::coordinate_distance_t(0, -1)},
        {GameDefinition::SOUTH, common::coordinate_distance_t(0, 1)},
        {GameDefinition::WEST, common::coordinate_distance_t(-1, 0)},
        {GameDefinition::EAST, common::coordinate_distance_t(1, 0)},

};

const std::unordered_map<const GameDefinition::Direction, const GameDefinition::Direction>
    GameLogic::map_direction_to_opposite_direction = {
        {GameDefinition::NORTH, GameDefinition::SOUTH},
        {GameDefinition::SOUTH, GameDefinition::NORTH},
        {GameDefinition::WEST, GameDefinition::EAST},
        {GameDefinition::EAST, GameDefinition::WEST},

};

Position GameLogic::PositionFromMovement(const Position &position, const coordinate_distance_t &movement,
                                         world::World &world) {
  Position new_position = position + movement;

  if (new_position.x < 0) {
    new_position.x += world.size.x;
  } else if (new_position.x >= world.size.x) {
    new_position.x -= world.size.x;
  }
  if (new_position.y < 0) {
    new_position.y += world.size.y;
  } else if (new_position.y >= world.size.y) {
    new_position.y -= world.size.y;
  }

  return new_position;
}

void GameLogic::MovePlayer(world::Player &player, const coordinate_distance_t &movement, world::World &world) {
  Position new_position = PositionFromMovement(player.position, movement, world);

  if (new_position != player.position && !world.IsBlocked(new_position)) {
    player.MoveTo(new_position);
  }

  // Check for collision with projectile
  auto projectile = world.GetProjectileFromPosition(new_position);

  if (projectile.has_value()) {
    // decrease player's health and remove projectile from world
    player.DecreaseHealth(projectile.value()->GetDamage());
    auto list = std::list<std::shared_ptr<Projectile>>();
    list.push_back(projectile.value());
    world.RemoveProjectiles(list);
  }
}

void GameLogic::HandleChange(world::Player &player, const Change &change, world::World &world) {
  if (!player.IsAlive()) {
    // If player is not alive, no changes are allowed
    return;
  }

  switch (change.GetChangeType()) {
    case ChangeType::MOVE_UP: {
      MovePlayer(player, coordinate_distance_t(0, -1), world);
      break;
    }
    case ChangeType::MOVE_LEFT: {
      MovePlayer(player, coordinate_distance_t(-1, 0), world);
      break;
    }
    case ChangeType::MOVE_RIGHT: {
      MovePlayer(player, coordinate_distance_t(1, 0), world);
      break;
    }
    case ChangeType::MOVE_DOWN: {
      MovePlayer(player, coordinate_distance_t(0, 1), world);
      break;
    }
    case ChangeType::USE_ITEM: {
      UseWeapon(player, world);
      break;
    }
    default:
      throw std::runtime_error("Unexpected ChangeType");
  }
}

void GameLogic::UseWeapon(Player &player, World &world) {
  auto weapon = player.GetWeapon();
  if (!weapon.has_value()) {
    return;
  }

  // TODO differentiate between items, right now only weapons expected
  Projectile bullet = weapon->SpawnProjectile(player.player_id, player.position, player.direction);
  world.AddProjectile(std::make_shared<Projectile>(bullet));
}

void GameLogic::MoveProjectile(Projectile &projectile, World &world) {
  auto movement = map_direction_to_movement.find(projectile.GetDirection())->second;
  auto new_position = PositionFromMovement(projectile.GetPosition(), movement, world);

  projectile.SetPosition(new_position, projectile.GetDirection());
  projectile.DecreaseRange();
}

std::list<std::shared_ptr<Projectile>> GameLogic::HandleProjectileCollision(
    const std::list<std::pair<std::shared_ptr<Projectile>, std::shared_ptr<Projectile>>> &projectile_pairs) {
  std::list<std::shared_ptr<Projectile>> destroy_projectile_list;
  std::list<std::shared_ptr<Projectile>> merge_projectile_list;

  for (auto &projectile_pair : projectile_pairs) {
    if (projectile_pair.first->GetPosition() == projectile_pair.second->GetPosition()) {
      // projectiles are at the same position after both movements

      if (projectile_pair.first->GetDirection() == projectile_pair.second->GetDirection()) {
        // projectiles have been shot multiple times in a row -> merge to one projectile_pair
        assert(projectile_pair.first->GetShooterId() == projectile_pair.second->GetShooterId());
        assert(projectile_pair.first->GetRange() == projectile_pair.second->GetRange());

        auto merge_projectile_it =
            std::find_if(merge_projectile_list.begin(), merge_projectile_list.end(),
                         [&projectile_pair](const std::shared_ptr<Projectile> &merge_projectile) {
                           return merge_projectile->GetPosition() == projectile_pair.second->GetPosition();
                         });

        if (merge_projectile_it == merge_projectile_list.end()) {
          destroy_projectile_list.push_back(projectile_pair.second);
          merge_projectile_list.push_back(projectile_pair.first);
          continue;
        }
      }

      // Remove collided projectiles
      destroy_projectile_list.push_back(projectile_pair.first);
      destroy_projectile_list.push_back(projectile_pair.second);

    } else {
      // Handle edge case: two projectiles with opposite directions passed each other
      // o-> <-o passed to <-o->
      auto opposite_direction =
          GameLogic::map_direction_to_opposite_direction.find(projectile_pair.second->GetDirection());
      if (projectile_pair.first->GetDirection() == opposite_direction->second) {
        destroy_projectile_list.push_back(projectile_pair.first);
      }
    }
  }

  return destroy_projectile_list;
}

void GameLogic::HandleProjectiles(World &world) {
  auto projectiles = world.GetProjectiles();
  std::list<std::shared_ptr<Projectile>> destroy_projectile_list;
  std::list<std::pair<std::shared_ptr<Projectile>, std::shared_ptr<Projectile>>> projectile_collision_list;

  for (auto &projectile : projectiles) {
    if (!projectile->StillFlying()) {
      destroy_projectile_list.push_back(projectile);
      continue;
    }

    // Move projectiles and search for hits
    MoveProjectile(*projectile, world);
    auto position = projectile->GetPosition();

    if (world.IsBlocked(position)) {
      // Projectile hit wall or player

      auto projectile_collided = HandleProjectileCollisionWithPlayer(projectile, world);
      if (projectile_collided) {
        destroy_projectile_list.push_back(projectile);
        continue;
      }
    }

    // Search for hit with other projectiles and add to collision list for handling
    auto projectiles_it = std::find_if(
        world.GetProjectiles().begin(), world.GetProjectiles().end(),
        [&position](const std::shared_ptr<Projectile> &projectile) { return projectile->GetPosition() == position; });

    if (projectiles_it != world.GetProjectiles().end() && projectiles_it->get() != projectile.get()) {
      projectile_collision_list.emplace_back(projectile, *projectiles_it);
    }
  }

  world.RemoveProjectiles(HandleProjectileCollision(projectile_collision_list));
  world.RemoveProjectiles(destroy_projectile_list);
}

bool GameLogic::HandleProjectileCollisionWithPlayer(std::shared_ptr<Projectile> &projectile, World &world) {
  // If a player caused collision, decrease health of player and remove projectile

  auto position = projectile->GetPosition();
  auto shot_player_it =
      std::find_if(world.players.begin(), world.players.end(),
                   [&position](const std::shared_ptr<Player> &player) { return player->position == position; });

  if (shot_player_it != world.players.end()) {
    // Check that shot player is still alive, otherwise no health or score changes are applied
    Player &hit_player = **shot_player_it;
    if (!hit_player.IsAlive()) {
      return false;
    }

    // Increase score of shooter and decrease health of shot player
    hit_player.DecreaseHealth(projectile->GetDamage());

    auto shooter = world.GetPlayerById(projectile->GetShooterId());
    if (shooter != nullptr && hit_player.player_id != projectile->GetShooterId()) {
      // If shooter is hit by own projectile, score is not increased!
      shooter->IncreaseScore(1);  // arbitrarily chosen number of points -> TODO associate with weapon?
    }
  }

  return true;
}
