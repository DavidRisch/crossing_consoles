#include <gtest/gtest.h>

#include <thread>

#include "../src/game/GameClient.h"
#include "../src/game/terminal/MockTerminal.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::terminal;

TEST(Game, NoAction) {
  auto player = std::make_shared<Player>("player name", Position(2, 2));
  World world(coordinate_size_t(5, 5));
  world.AddWall(Position(4, 4));

  auto mock_terminal = std::make_shared<MockTerminal>();
  mock_terminal->AddInput((char)KeyCode::ESCAPE);

  GameClient gc(player, mock_terminal, coordinate_size_t(10, 10));
  gc.Run();

  bool empty = true;
  for (const auto& i_lines : mock_terminal->GetLastOutput()) {
    for (const auto& i_characters : i_lines) {
      if (i_characters != ColoredChar(L' ', Color::WHITE, Color::BLACK)) {
        empty = false;
      }
    }
  }

  ASSERT_FALSE(empty);
}

TEST(Game, Actions) {
  auto player = std::make_shared<Player>("player name", Position(2, 2));
  World world(coordinate_size_t(5, 5));
  world.AddWall(Position(4, 4));

  auto mock_terminal = std::make_shared<MockTerminal>();
  mock_terminal->AddInput((char)KeyCode::W);
  mock_terminal->AddInput((char)KeyCode::A);
  mock_terminal->AddInput((char)KeyCode::S);
  mock_terminal->AddInput((char)KeyCode::D);
  mock_terminal->AddInput((char)KeyCode::ESCAPE);

  GameClient gc(player, mock_terminal, coordinate_size_t(10, 10));
  gc.Run();

  bool empty = true;
  for (const auto& i_lines : mock_terminal->GetLastOutput()) {
    for (const auto& i_characters : i_lines) {
      if (i_characters != ColoredChar(L' ', Color::WHITE, Color::BLACK)) {
        empty = false;
      }
    }
  }

  ASSERT_FALSE(empty);
}

TEST(Game, ActionShoot) {
  // this needs to be adapted if viewport header changes!
  auto header_size = common::coordinate_size_t(2, 7);

  auto player = std::make_shared<Player>("player name", Position(3, 2));
  player->direction = GameDefinition::NORTH;
  World world(coordinate_size_t(5, 5));
  world.AddWall(Position(4, 4));

  auto mock_terminal = std::make_shared<MockTerminal>();
  mock_terminal->AddInput((char)KeyCode::SPACE);
  mock_terminal->AddInput((char)KeyCode::ESCAPE);

  GameClient gc(player, mock_terminal, coordinate_size_t(10, 10));
  gc.Run();

  // projectile moves NORTH
  auto projectile_position = Position(3, 1);

  int row = 0;
  bool contains_bullet = false;

  for (const auto& i_lines : mock_terminal->GetLastOutput()) {
    int column = 0;
    for (const auto& i_characters : i_lines) {
      if (i_characters == ColoredChar(L'o', Color::WHITE, Color::BLACK)) {
        ASSERT_EQ(Position((column / 2) - header_size.x, row - header_size.y), projectile_position);
        return;
      }
      column++;
    }
    row++;
  }

  ASSERT_TRUE(contains_bullet);
}
