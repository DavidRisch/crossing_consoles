#include <gtest/gtest.h>

#include <thread>

#include "../src/game/GameClient.h"
#include "../src/game/terminal/MockTerminal.h"

char escape_key = 27;

TEST(Game, NoAction) {
  Player player("player name", Position(2, 2));
  World world(5, 5);
  world.AddWall(Position(4, 4));

  auto mock_terminal = std::make_shared<MockTerminal>();
  mock_terminal->AddInput(escape_key);

  GameClient gc(player, world, mock_terminal);

  ASSERT_FALSE(mock_terminal->GetLastOutput().empty());
}

TEST(Game, Actions) {
  Player player("player name", Position(2, 2));
  World world(5, 5);
  world.AddWall(Position(4, 4));

  auto mock_terminal = std::make_shared<MockTerminal>();
  mock_terminal->AddInput('w');
  mock_terminal->AddInput('a');
  mock_terminal->AddInput('s');
  mock_terminal->AddInput('d');
  mock_terminal->AddInput(escape_key);

  GameClient gc(player, world, mock_terminal);

  ASSERT_FALSE(mock_terminal->GetLastOutput().empty());
}
