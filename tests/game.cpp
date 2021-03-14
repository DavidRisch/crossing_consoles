#include <gtest/gtest.h>

#include <thread>

#include "../src/game/GameClient.h"
#include "../src/game/terminal/MockTerminal.h"

TEST(Game, NoAction) {
  Player player("player name", Position(2, 2));
  World world(coordinate_size_t(5, 5));
  world.AddWall(Position(4, 4));

  auto mock_terminal = std::make_shared<MockTerminal>();
  mock_terminal->AddInput((char)KeyCode::ESCAPE);

  GameClient gc(player, world, mock_terminal);

  ASSERT_FALSE(mock_terminal->GetLastOutput().empty());
}

TEST(Game, Actions) {
  Player player("player name", Position(2, 2));
  World world(coordinate_size_t(5, 5));
  world.AddWall(Position(4, 4));

  auto mock_terminal = std::make_shared<MockTerminal>();
  mock_terminal->AddInput((char)KeyCode::W);
  mock_terminal->AddInput((char)KeyCode::A);
  mock_terminal->AddInput((char)KeyCode::S);
  mock_terminal->AddInput((char)KeyCode::D);
  mock_terminal->AddInput((char)KeyCode::ESCAPE);

  GameClient gc(player, world, mock_terminal);

  ASSERT_FALSE(mock_terminal->GetLastOutput().empty());
}
