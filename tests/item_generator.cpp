#include <gtest/gtest.h>

#include "../src/game/world/EmptyWorldGenerator.h"
#include "../src/game/world/items/ItemGenerator.h"

using namespace game::world;
using namespace game::common;

TEST(ItemGenerator, GenerateItem) {
  std::shared_ptr<World> world = EmptyWorldGenerator().GenerateWorld(coordinate_size_t(20, 15));
  ASSERT_TRUE(world->items.empty());
  world->GetItemGenerator().GenerateItem();
  ASSERT_FALSE(world->items.empty());
}
