#include "ItemGenerator.h"

#include <random>

#include "../World.h"
#include "Gun.h"
#include "Heart.h"
#include "Points.h"
#include "Sword.h"

using namespace game;
using namespace world;
using namespace common;

std::random_device ItemGenerator::random_device;

ItemGenerator::ItemGenerator(World* world)
    : world(world) {
}

void ItemGenerator::GenerateItem() {
  if (world->items.size() == GameDefinition::max_items_in_world) {
    return;
  }

  // Generate random number between 0 and 3
  std::uniform_int_distribution<int> distribution(0, 3);
  int random = distribution(random_device);

  // Generate a new item whose type is determined by the random number
  auto item = std::shared_ptr<IItem>();
  switch (random) {
    case 0:
      item = std::make_shared<Gun>(1, 20);
      break;
    case 1:
      item = std::make_shared<Sword>(2);
      break;
    case 2:
      item = std::make_shared<Heart>(1);
      break;
    case 3:
      item = std::make_shared<Points>(10);
      break;
  }

  Position generated_position = world->GetSpawner().GenerateSpawnPosition();

  world->AddItem(generated_position, item);
}
