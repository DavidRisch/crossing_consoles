#include "GameLogic.h"

#include <algorithm>
#include <cassert>

#include "world/Projectile.h"
#include "world/items/Gun.h"
#include "world/items/Sword.h"

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

Position GameLogic::AttackedPositionFromDirection(const Position &position, const GameDefinition::Direction direction) {
  Position attacked_position = Position(0, 0);
  switch (direction) {
    case GameDefinition::NORTH:
      attacked_position.Set(position.x, position.y - 1);
      break;
    case GameDefinition::EAST:
      attacked_position.Set(position.x + 1, position.y);
      break;
    case GameDefinition::SOUTH:
      attacked_position.Set(position.x, position.y + 1);
      break;
    case GameDefinition::WEST:
      attacked_position.Set(position.x - 1, position.y);
      break;
  }
  return attacked_position;
}

void GameLogic::MovePlayer(world::Player &player, const coordinate_distance_t &movement, world::World &world) {
  Position new_position = PositionFromMovement(player.position, movement, world);

  if (new_position != player.position && !world.IsBlocked(new_position)) {
    player.MoveTo(new_position);
  }

  // Pick up item if there is one at the new position
  if (world.items.find(player.position) != world.items.end()) {
    player.SetItem(world.items.at(player.position));
    world.items.erase(world.items.find(player.position));
  }

  // Check for collision with projectile
  auto projectile = world.GetProjectileFromPosition(new_position);

  if (projectile.has_value()) {
    if ((*projectile)->CanCollideWithPlayer(player)) {
      // Show hit position in color
      world.AddColoredField(ColoredField(player.position));

      // decrease player's health and remove projectile from world
      ApplyDamageToPlayer(player, projectile.value()->GetDamage());
      auto list = std::list<std::shared_ptr<Projectile>>();
      list.push_back(projectile.value());
      world.RemoveProjectiles(list);
    }
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
      player.direction = GameDefinition::NORTH;
      break;
    }
    case ChangeType::MOVE_LEFT: {
      MovePlayer(player, coordinate_distance_t(-1, 0), world);
      player.direction = GameDefinition::WEST;
      break;
    }
    case ChangeType::MOVE_RIGHT: {
      MovePlayer(player, coordinate_distance_t(1, 0), world);
      player.direction = GameDefinition::EAST;
      break;
    }
    case ChangeType::MOVE_DOWN: {
      MovePlayer(player, coordinate_distance_t(0, 1), world);
      player.direction = GameDefinition::SOUTH;
      break;
    }
    case ChangeType::USE_ITEM: {
      UseWeapon(player, world);
      break;
    }
    case ChangeType::USE_UP: {
      player.direction = GameDefinition::NORTH;
      UseWeapon(player, world);
      break;
    }
    case ChangeType::USE_LEFT: {
      player.direction = GameDefinition::WEST;
      UseWeapon(player, world);
      break;
    }
    case ChangeType::USE_RIGHT: {
      player.direction = GameDefinition::EAST;
      UseWeapon(player, world);
      break;
    }
    case ChangeType::USE_DOWN: {
      player.direction = GameDefinition::SOUTH;
      UseWeapon(player, world);
      break;
    }
    default:
      throw std::runtime_error("Unexpected ChangeType");
  }
}

void GameLogic::UseWeapon(Player &player, World &world) {
  auto weapon = player.GetWeapon();
  if (weapon == nullptr) {
    return;
  }

  switch (weapon->GetWeaponType()) {
    case WeaponType::GUN: {
      Projectile bullet =
          std::dynamic_pointer_cast<Gun>(weapon)->SpawnProjectile(player.player_id, player.position, player.direction);
      world.AddProjectile(std::make_shared<Projectile>(bullet));
      break;
    }
    case WeaponType::SWORD: {
      std::shared_ptr<Sword> sword = std::dynamic_pointer_cast<Sword>(weapon);
      Position attacked_position = AttackedPositionFromDirection(player.position, player.direction);

      // Add color to field on which damage was caused
      world.AddColoredField(ColoredField(attacked_position));

      // check whether another player has been hit
      auto hit_player_it = std::find_if(world.players.begin(), world.players.end(),
                                        [&attacked_position](const std::shared_ptr<Player> &other_player) {
                                          return other_player->position == attacked_position;
                                        });

      if (hit_player_it != world.players.end()) {
        // Check that hit player is still alive, otherwise no health or score changes are applied
        Player &hit_player = **hit_player_it;
        if (!hit_player.IsAlive()) {
          return;
        }
        // Increase score of player and decrease health of hit player
        ApplyDamageToPlayer(hit_player, sword->GetDamage());
        player.IncreaseScore();
      }

      // destroy hit projectiles
      for (auto &projectile : world.GetProjectiles()) {
        if (attacked_position == projectile->GetPosition()) {
          std::list<std::shared_ptr<Projectile>> hit_projectile = {projectile};
          world.RemoveProjectiles(hit_projectile);
        }
      }
      break;
    }
  }
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

  for (auto &projectile_pair : projectile_pairs) {
    if (projectile_pair.first->GetPosition() == projectile_pair.second->GetPosition()) {
      // projectiles are at the same position after both movements

      if (projectile_pair.first->GetDirection() == projectile_pair.second->GetDirection() &&
          projectile_pair.first->GetShooterId() == projectile_pair.second->GetShooterId()) {
        // projectiles have been shot multiple times in a row -> this is prevented by World::AddProjectile()
        assert(false);
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

    if (projectiles_it != world.GetProjectiles().end() && *projectiles_it != projectile) {
      // check if collided projectile is marked to be destroyed
      auto projectile_to_destroy_it =
          std::find_if(destroy_projectile_list.begin(), destroy_projectile_list.end(),
                       [&projectiles_it](const std::shared_ptr<Projectile> &projectile_to_destroy) {
                         return projectile_to_destroy == *projectiles_it;
                       });

      if (projectile_to_destroy_it == destroy_projectile_list.end()) {
        // projectile collided with another, still flying projectile
        projectile_collision_list.emplace_back(projectile, *projectiles_it);
      }
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

    if (!projectile->CanCollideWithPlayer(hit_player)) {
      return false;
    }

    // Show hit position in color
    world.AddColoredField(ColoredField(hit_player.position));

    // Increase score of shooter and apply damage to shot player
    ApplyDamageToPlayer(hit_player, projectile->GetDamage());

    auto shooter = world.GetPlayerById(projectile->GetShooterId());
    if (shooter != nullptr && hit_player.player_id != projectile->GetShooterId()) {
      // If shooter is hit by own projectile, score is not increased!
      shooter->IncreaseScore();
    }
  }

  return true;
}

void GameLogic::ApplyDamageToPlayer(Player &player, int damage) {
  player.DecreaseHealth(damage);
  if (!player.IsAlive()) {
    player.Die();
  }
}

void GameLogic::HandlePlayerRespawn(Player &player, World &world,
                                    const std::chrono::duration<int64_t, std::milli> respawn_time) {
  if (!player.IsAlive() && std::chrono::steady_clock::now() - player.time_of_death > respawn_time) {
    world.ResurrectPlayer(player);
  }
}

void GameLogic::ReduceColoredFieldLifetimes(World &world) {
  std::list<Position> delete_list;

  for (auto &field_it : world.colored_fields) {
    field_it.second.ReduceLifetime();
    if (field_it.second.GetLifetime() == 0) {
      delete_list.push_back(field_it.first);
    }
  }

  for (const auto &delete_position : delete_list) {
    world.colored_fields.erase(delete_position);
  }
}
