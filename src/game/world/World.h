#ifndef CROSSING_CONSOLES_WORLD_H
#define CROSSING_CONSOLES_WORLD_H

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../common/Position.h"
#include "Player.h"
#include "Projectile.h"
#include "Spawner.h"
#include "Wall.h"
#include "items/ItemGenerator.h"

namespace game::world {

typedef std::list<std::shared_ptr<Player>> player_ptr_list_t;

/**
 * \brief Handles information about the world: its size, players and walls.
 */
class World : public networking::ISerializable {
 public:
  common::coordinate_size_t size;
  player_ptr_list_t players{};
  std::unordered_map<common::Position, Wall, common::Position::HashFunction> walls;
  bool updated = false;
  std::unordered_map<common::Position, std::shared_ptr<IItem>, common::Position::HashFunction> items;

  explicit World(common::coordinate_size_t size);

  void AddPlayer(const std::shared_ptr<Player>& player);

  void RemovePlayer(GameDefinition::player_id_t player_id);

  void AddWall(const common::Position& position);

  void AddItem(const common::Position& position, const std::shared_ptr<IItem>& item);

  void AddProjectile(const std::shared_ptr<Projectile>& projectile);

  std::list<std::shared_ptr<Projectile>> GetProjectiles();

  /**
   * \brief Remove all given `Projectile`s from world's `projectiles`.
   */
  void RemoveProjectiles(std::list<std::shared_ptr<Projectile>> list_to_remove);

  /**
   * \brief Returns a `Projectile` with the given position if it exists.
   */
  std::optional<std::shared_ptr<Projectile>> GetProjectileFromPosition(common::Position position);

  bool IsBlocked(const common::Position& position);

  bool IsBlockedForItem(const common::Position& position);

  /**
   * \brief Replace some data with newer data contained in `server_world`.
   */
  void Update(const World& server_world);

  std::shared_ptr<Player> GetPlayerById(int player_id) const;

  const Spawner& GetSpawner() const;

  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static World Deserialize(std::vector<uint8_t>::iterator& input_iterator);

  /**
   * \brief Respawn player in world, reset score and restore health
   */
  void ResurrectPlayer(Player& player);

  ItemGenerator GetItemGenerator();

 private:
  Spawner spawner;
  ItemGenerator item_generator;
  std::list<std::shared_ptr<Projectile>> projectiles{};
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_WORLD_H
