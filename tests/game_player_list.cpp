#include <gtest/gtest.h>

#include "../src/game/GameClient.h"
#include "fixtures/GamePlayerList.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::terminal;

TEST_F(GamePlayerList, OnePlayer) {
  std::string player_name = "abcABC";
  auto player = std::make_shared<Player>(player_name, Position(0, 0));
  world::player_ptr_list_t players{player};

  PlayerList player_list(players);

  auto result = player_list.Render();
  EXPECT_TRUE(result.Find(ToWstring(player_name)).has_value());
}

TEST_F(GamePlayerList, ManyPlayers) {
  world::player_ptr_list_t players;
  for (int i = 0; i < 100; ++i) {
    auto player = std::make_shared<Player>(std::string("PLAyer ") + std::to_string(i), Position(0, 0));
  }

  PlayerList player_list(players);

  auto result = player_list.Render();
  for (const auto& player : players) {
    EXPECT_TRUE(result.Find(ToWstring(player->name)).has_value());
  }
}

TEST_F(GamePlayerList, Compositor) {
  std::string player_name = "abcABC";
  auto player = std::make_shared<Player>(player_name, Position(0, 0));
  World world(coordinate_size_t(5, 5));
  world.AddPlayer(player);

  // a small viewport_size causes errors
  coordinate_size_t viewport_size = Position(100, 100);
  Compositor compositor(viewport_size, world, *player);

  {
    auto result = compositor.CompositeViewport();
    EXPECT_FALSE(result.Find(ToWstring(player_name)).has_value());
  }
  {
    compositor.show_player_list = true;
    auto result = compositor.CompositeViewport();
    EXPECT_TRUE(result.Find(ToWstring(player_name)).has_value());
  }
}
