#include "ItemGenerator.h"

#include <cstdlib>
#include <ctime>
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
  std::random_device generator;
  std::uniform_int_distribution<int> distribution(0, 3);
  int random = distribution(generator);
  auto item = std::shared_ptr<IItem>();
  switch (random) {
    case 0:
      item = std::make_shared<Gun>(1, 20);
      break;
    case 1:
      item = std::make_shared<Sword>(1, 1);
      break;
    case 2:
      item = std::make_shared<Heart>(-1);
      break;
    case 3:
      item = std::make_shared<Points>(10);
      break;
  }

  common::Position generated_position = common::Position(0, 0);
  do {
    // TODO: avoid possible endless loop
    std::uniform_int_distribution<int> x_coordinate(0, world->size.x);
    int random_x = distribution(generator);
    std::uniform_int_distribution<int> y_coordinate(0, world->size.y);
    int random_y = distribution(generator);
    generated_position = common::Position(random_x, random_y);
  } while (world->IsBlockedForItem(generated_position));

  world->AddItem(generated_position, item);
}
