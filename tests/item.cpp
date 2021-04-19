#include <gtest/gtest.h>

#include "../src/game/world/EmptyWorldGenerator.h"
#include "../src/game/world/items/Gun.h"
#include "../src/game/world/items/Heart.h"
#include "../src/game/world/items/Points.h"
#include "../src/game/world/items/Sword.h"
#include "fixtures/Items.h"

using namespace game;
using namespace game::world;
using namespace game::common;

TEST_F(Items, GetItemType) {
  auto gun = std::make_shared<Gun>(1, 20);
  ASSERT_EQ(gun->GetItemType(), ItemType::WEAPON);
  ASSERT_EQ(gun->GetWeaponType(), WeaponType::GUN);

  auto sword = std::make_shared<Sword>(2);
  ASSERT_EQ(sword->GetItemType(), ItemType::WEAPON);
  ASSERT_EQ(sword->GetWeaponType(), WeaponType::SWORD);

  auto heart = std::make_shared<Heart>(1);
  ASSERT_EQ(heart->GetItemType(), ItemType::HEART);

  auto points = std::make_shared<Points>(10);
  ASSERT_EQ(points->GetItemType(), ItemType::POINTS);
}

TEST_F(Items, Serialization) {
  std::vector<uint8_t> serialization_vector;
  auto gun = std::make_shared<Gun>(1, 20);
  gun->Serialize(serialization_vector);

  auto sword = std::make_shared<Sword>(2);
  sword->Serialize(serialization_vector);

  auto heart = std::make_shared<Heart>(1);
  heart->Serialize(serialization_vector);

  auto points = std::make_shared<Points>(10);
  points->Serialize(serialization_vector);

  ASSERT_FALSE(serialization_vector.empty());

  auto iterator = serialization_vector.begin();

  auto gun_new = Gun::Deserialize(iterator);
  EXPECT_EQ(gun_new->GetDamage(), gun->GetDamage());
  EXPECT_EQ(gun_new->GetRange(), gun->GetRange());

  auto sword_new = Sword::Deserialize(iterator);
  EXPECT_EQ(sword_new->GetDamage(), sword->GetDamage());

  auto heart_new = Heart::Deserialize(iterator);
  EXPECT_EQ(heart_new->GetHealing(), heart->GetHealing());

  auto points_new = Points::Deserialize(iterator);
  EXPECT_EQ(points_new->GetValue(), points->GetValue());

  ASSERT_EQ(iterator, serialization_vector.end());
}

TEST_F(Items, SwordCausesDamage) {
  generate_world();
  add_two_players();

  auto sword = std::make_shared<Sword>(2);
  player_first->SetItem(sword);

  int health_before_hit = player_second->health;

  action_with_player(player_first, game::networking::ChangeType::MOVE_RIGHT, 1);
  action_with_player(player_first, game::networking::ChangeType::USE_ITEM, 1);

  ASSERT_EQ(player_second->health, health_before_hit - sword->GetDamage());
}

TEST_F(Items, SwordDestroysProjectile) {
  generate_world();
  add_one_player();

  auto sword = std::make_shared<Sword>(2);
  player_first->SetItem(sword);

  auto projectile = std::make_shared<Projectile>(20, 1, 0, Position(1, 0), game::GameDefinition::WEST);
  world->AddProjectile(projectile);

  ASSERT_EQ(player_first->position, Position(0, 0));
  ASSERT_EQ(player_first->direction, GameDefinition::EAST);
  ASSERT_EQ(projectile->GetPosition(), Position(1, 0));
  ASSERT_EQ(projectile->GetDirection(), GameDefinition::WEST);

  ASSERT_FALSE(world->GetProjectiles().empty());

  action_with_player(player_first, game::networking::ChangeType::USE_ITEM, 1);

  ASSERT_TRUE(world->GetProjectiles().empty());
}

TEST_F(Items, Heart) {
  generate_world();
  add_one_player();

  player_first->DecreaseHealth(1);
  int health_with_damage = player_first->health;

  auto heart = std::make_shared<Heart>(1);
  world->AddItem(Position(1, 0), heart);
  EXPECT_FALSE(world->items.empty());

  action_with_player(player_first, game::networking::ChangeType::MOVE_RIGHT, 1);

  ASSERT_EQ(player_first->health, health_with_damage + heart->GetHealing());
  ASSERT_TRUE(world->items.empty());
}

TEST_F(Items, Points) {
  generate_world();
  add_one_player();

  int old_score = player_first->score;

  auto points = std::make_shared<Points>(10);
  world->AddItem(Position(1, 0), points);
  EXPECT_FALSE(world->items.empty());

  action_with_player(player_first, game::networking::ChangeType::MOVE_RIGHT, 1);

  ASSERT_EQ(player_first->score, old_score + points->GetValue());
  ASSERT_TRUE(world->items.empty());
}

TEST_F(Items, PickUpWeapons) {
  generate_world();
  add_one_player();

  EXPECT_EQ(player_first->GetWeapon(), nullptr);

  auto sword = std::make_shared<Sword>(2);
  world->AddItem(Position(1, 0), sword);
  auto gun = std::make_shared<Gun>(1, 20);
  world->AddItem(Position(2, 0), gun);
  EXPECT_FALSE(world->items.empty());

  action_with_player(player_first, game::networking::ChangeType::MOVE_RIGHT, 1);
  ASSERT_EQ(player_first->GetWeapon(), sword);
  action_with_player(player_first, game::networking::ChangeType::MOVE_RIGHT, 1);
  ASSERT_EQ(player_first->GetWeapon(), gun);

  ASSERT_TRUE(world->items.empty());
}
