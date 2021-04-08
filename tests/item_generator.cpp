#include <gtest/gtest.h>
#include "../src/game/world/items/ItemGenerator.h"
#include "../src/game/world/WorldGenerator.h"

using namespace game::world;
using namespace game::common;

TEST(ItemGenerator, GenerateItem){
  std::shared_ptr<World> world = WorldGenerator::GenerateWorld(coordinate_size_t(20, 15));
  ASSERT_TRUE(world->items.empty());
  world->GetItemGenerator().GenerateItem();
  ASSERT_FALSE(world->items.empty());
}