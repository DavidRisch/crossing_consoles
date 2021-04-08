#include "ItemGenerator.h"

#include <random>
#include <utility>

#include "Gun.h"
#include "Heart.h"
#include "Points.h"
#include "Sword.h"

using namespace game;
using namespace world;

ItemGenerator::ItemGenerator(std::shared_ptr<World> world)
    : world(std::move(world)) {
}

void ItemGenerator::GenerateItem() {
  // Generate random number between 0 and 3
  std::random_device generator;
  std::uniform_int_distribution<int> distribution(0, 3);
  int random = distribution(generator);

  // Generate a new item whose type is determined by the random number
  auto item = std::shared_ptr<IItem>();
  switch (random) {
    case 0:
      item = std::make_shared<Gun>(GameDefinition::gun_damage, GameDefinition::gun_range);
      break;
    case 1:
      item = std::make_shared<Sword>(GameDefinition::sword_damage);
      break;
    case 2:
      item = std::make_shared<Heart>(GameDefinition::heart_healing);
      break;
    case 3:
      item = std::make_shared<Points>(GameDefinition::points_value);
      break;
  }

  // Generate a position for the item which is not blocked by a wall, player or other item
  common::Position generated_position = common::Position(0, 0);
  do {
    // TODO: avoid possible endless loop -> throw exception at some point
    std::uniform_int_distribution<int> x_coordinate(0, world->size.x);
    int random_x = distribution(generator);
    std::uniform_int_distribution<int> y_coordinate(0, world->size.y);
    int random_y = distribution(generator);
    generated_position = common::Position(random_x, random_y);
  } while (world->IsBlockedForItem(generated_position));

  world->AddItem(generated_position, item);
}
