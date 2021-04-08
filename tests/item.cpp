#include <gtest/gtest.h>

#include "../src/game/world/WorldGenerator.h"
#include "../src/game/world/items/Heart.h"
#include "../src/game/world/items/ItemGenerator.h"
#include "../src/game/world/items/Points.h"
#include "../src/game/world/items/Sword.h"

using namespace game::world;
using namespace game::common;

TEST(Item, GetItemType) {
  auto gun = std::make_shared<Gun>(1, 20);
  ASSERT_EQ(gun->GetItemType(), ItemType::GUN);

  auto sword = std::make_shared<Sword>(2);
  ASSERT_EQ(sword->GetItemType(), ItemType::SWORD);

  auto heart = std::make_shared<Heart>(1);
  ASSERT_EQ(heart->GetItemType(), ItemType::HEART);

  auto points = std::make_shared<Points>(10);
  ASSERT_EQ(points->GetItemType(), ItemType::POINTS);
}

TEST(Item, Serialize){

}