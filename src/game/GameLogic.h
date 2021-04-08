#ifndef CROSSING_CONSOLES_GAMELOGIC_H
#define CROSSING_CONSOLES_GAMELOGIC_H

#include "GameDefinition.h"
#include "networking/Change.h"
#include "world/World.h"

namespace game {

/**
 * \brief Apply game logic to `World` and `Player` on server and client side
 */
class GameLogic {
 public:
  /**
   * \brief Handle a `Change` produced by a `GameClient` controlling the specific player or by keyboard input.
   */
  static void HandleChange(world::Player &player, const networking::Change &change, world::World &world);

  /**
   * \brief Handle respawn of dead `Player` in `World`.
   */
  static void HandlePlayerRespawn(world::Player &player, world::World &world);

  /**
   * \brief Handle `Projectile`s movement and collision with `Player`s or `Wall`s in `World`.
   */
  static void HandleProjectiles(world::World &world);

 private:
  /**
   * \brief Move the player with the given `movement`.
   * \details Handles collisions with 'Projectile`s and items during movement.
   */
  static void MovePlayer(world::Player &player, const common::coordinate_distance_t &movement, world::World &world);

  /**
   * \brief Move the `Projectile` using its direction.
   */
  static void MoveProjectile(Projectile &projectile, world::World &world);

  /**
   * \brief Player uses weapon.
     \details Spawns `Projectile` in world for `Gun`.
   */
  static void UseWeapon(world::Player &player, world::World &world);

  static const std::unordered_map<const GameDefinition::Direction, common::coordinate_distance_t>
      map_direction_to_movement;

  static const std::unordered_map<const GameDefinition::Direction, const GameDefinition::Direction>
      map_direction_to_opposite_direction;

  /**
   * \brief Return new position in world from given movement.
   */
  static common::Position PositionFromMovement(const common::Position &position,
                                               const common::coordinate_distance_t &movement, world::World &world);

  /**
   * \brief Checks each `Projectile` pair in `projectile_pairs` for collisions.
   * \details Returns list of collided projectiles that need to be removed from 'World'
   */
  static std::list<std::shared_ptr<Projectile>> HandleProjectileCollision(
      const std::list<std::pair<std::shared_ptr<Projectile>, std::shared_ptr<Projectile>>> &projectile_pairs);

  /**
   * \brief Handle collision of a `Projectile` collision with a `Player`.
   * \details Check if a player is shot. If the shot player is alive, their health is decreased, the projectile removed
   * and the shooter's score increased.
   */
  static bool HandleProjectileCollisionWithPlayer(std::shared_ptr<Projectile> &projectiles, world::World &world);

  /**
   * \brief Returns the position that gets attacked with a sword when the position and the direction of a player are
   * given.
   */
  static common::Position AttackedPositionFromDirection(const common::Position &position,
                                                        GameDefinition::Direction direction);

  /**
   * \brief Apply given `healing` to `Player`and invoke Death if necessary
   */
  static void ApplyDamageToPlayer(world::Player &player, int damage);
};

}  // namespace game
#endif  // CROSSING_CONSOLES_GAMELOGIC_H
