#include "GameLogic.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::networking;

void GameLogic::MovePlayer(world::Player &player, const coordinate_distance_t &movement, world::World &world) {
  Position new_position = player.position + movement;

  if (new_position.x < 0) {
    new_position.x += world.size.x;
  } else if (new_position.x >= world.size.x) {
    new_position.x -= world.size.x;
  }
  if (new_position.y < 0) {
    new_position.y += world.size.y;
  } else if (new_position.y >= world.size.y) {
    new_position.y -= world.size.y;
  }

  if (new_position != player.position && !world.IsBlocked(new_position)) {
    player.MoveTo(new_position);
  }
}

void GameLogic::HandleChange(world::Player &player, const Change &change, world::World &world) {
  switch (change.GetChangeType()) {
    case ChangeType::MOVE_UP: {
      MovePlayer(player, coordinate_distance_t(0, -1), world);
      break;
    }
    case ChangeType::MOVE_LEFT: {
      MovePlayer(player, coordinate_distance_t(-1, 0), world);
      break;
    }
    case ChangeType::MOVE_RIGHT: {
      MovePlayer(player, coordinate_distance_t(1, 0), world);
      break;
    }
    case ChangeType::MOVE_DOWN: {
      MovePlayer(player, coordinate_distance_t(0, 1), world);
      break;
    }
    default:
      throw std::runtime_error("Unexpected ChangeType");
  }
}
