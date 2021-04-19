#include <gtest/gtest.h>

#include "../src/game/GameClient.h"
#include "../src/game/GameLogic.h"
#include "../src/game/world/items/Gun.h"

using namespace game;
using namespace game::common;
using namespace game::world;

TEST(Projectile, CreateFromWeapon) {
  // create projectile from weapon

  int damage = 10;
  uint8_t range = 3;

  auto player = std::make_shared<Player>("player", Position(0, 0));
  auto weapon = Gun(damage, range);
  Projectile projectile = weapon.SpawnProjectile(player->player_id, player->position, player->direction);

  ASSERT_EQ(projectile.GetDamage(), damage);
  ASSERT_EQ(projectile.GetShooterId(), player->player_id);
  ASSERT_EQ(projectile.GetPosition(), player->position);
  ASSERT_EQ(projectile.GetDirection(), player->direction);

  // range of projectile always needs to be +1 than range og weapon
  ASSERT_EQ(projectile.GetRange(), range + 1);
  ASSERT_TRUE(projectile.StillFlying());

  for (int i = 0; i < range + 1; i++) {
    projectile.DecreaseRange();
  }

  ASSERT_FALSE(projectile.StillFlying());
}

TEST(Projectile, HitPlayer) {
  // Decrease health of player using projectile

  // choose any damage/ range > 0
  int damage = 10;
  uint8_t range = 3;

  auto player = std::make_shared<Player>("player", Position(0, 0));

  auto weapon = Gun(damage, range);
  Projectile projectile = weapon.SpawnProjectile(player->player_id, player->position, player->direction);

  player->DecreaseHealth(projectile.GetDamage());
  ASSERT_EQ(player->health, player->max_health - damage);
}

TEST(Projectile, Spawn) {
  // spawn projectile and add to world
  World world(coordinate_size_t(21, 21));
  auto player = std::make_shared<Player>("player", Position(0, 0));
  player->SetItem(std::make_shared<Gun>(1, 20));

  world.AddPlayer(player);

  ASSERT_TRUE(world.players.size() == 1);
  EXPECT_EQ(world.GetPlayerById(player->player_id), player);

  game::networking::Change item_change(game::networking::ChangeType::USE_ITEM);
  GameLogic::HandleChange(*player, item_change, world);

  ASSERT_TRUE(world.GetProjectiles().size() == 1);
}
