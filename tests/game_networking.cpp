#include <gtest/gtest.h>

#include <mutex>
#include <thread>

#include "../src/game/GameClient.h"
#include "../src/game/GameServer.h"
#include "../src/game/terminal/MockTerminal.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::terminal;

class GameNetworking : public ::testing::Test {
 public:
  GameNetworking() = default;

  std::shared_ptr<GameServer> game_server;

  std::vector<std::shared_ptr<GameClient>> game_clients;
  std::vector<std::shared_ptr<MockTerminal>> mock_terminals;

  bool keep_server_running = false;
  std::shared_ptr<std::thread> server_thread;

  long server_iteration_count = 0;

  communication::ProtocolDefinition::timeout_t communication_timeout = std::chrono::milliseconds(100);

  void start_server() {
    assert(server_thread == nullptr);

    keep_server_running = true;

    server_iteration_count = 0;

    server_thread = std::make_shared<std::thread>([this] {
      while (keep_server_running) {
        game_server->RunIteration();
        server_iteration_count++;
        std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
    });
  }

  void stop_server() {
    assert(server_thread != nullptr);

    keep_server_running = false;
    server_thread->join();
    server_thread.reset();
  }

  void run_clients() {
    std::list<std::thread> threads;
    for (auto& game_client : game_clients) {
      threads.emplace_back([&game_client]() { game_client->Run(); });
    }

    for (auto& thread : threads) {
      thread.join();
    }
  }

  void create_server_and_client() {
    game_server = std::make_shared<GameServer>(coordinate_size_t(35, 45), communication_timeout);

    create_new_client(Position(1, 5));
  }

  void create_new_client(const Position& position) {
    auto player = std::make_shared<Player>("user", position);
    mock_terminals.push_back(std::make_shared<MockTerminal>());

    start_server();
    game_clients.push_back(std::make_shared<GameClient>(player, mock_terminals.back(), coordinate_size_t(1, 1), true,
                                                        communication_timeout));
    stop_server();
  }

  void wait_a_few_iterations() const {
    auto target_iteration = server_iteration_count + 10;
    while (server_iteration_count < target_iteration) {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  }

  static void add_inputs(MockTerminal& terminal) {
    terminal.AddInput((char)KeyCode::W);
    terminal.AddInput((char)KeyCode::W);
    terminal.AddInput((char)KeyCode::A);
    terminal.AddInput((char)KeyCode::A);
    terminal.AddInput((char)KeyCode::S);
    terminal.AddInput((char)KeyCode::D);
  }

  void expect_some_output_on_all() {
    wait_for_renderer();
    for (const auto& mock_terminal : mock_terminals) {
      EXPECT_FALSE(mock_terminal->GetLastOutput().empty());
    }
  }

  void wait_for_renderer() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
};

TEST_F(GameNetworking, NoAction) {
  create_server_and_client();

  mock_terminals.at(0)->AddInput((char)KeyCode::ESCAPE);

  start_server();
  run_clients();
  stop_server();

  expect_some_output_on_all();
}

TEST_F(GameNetworking, Actions) {
  create_server_and_client();

  start_server();

  std::thread input_thread([this] {
    {
      auto old_position = game_server->GetWorld().players.front()->position;
      mock_terminals.at(0)->AddInput((char)KeyCode::S);

      wait_a_few_iterations();
      auto new_position = game_server->GetWorld().players.front()->position;

      EXPECT_EQ(new_position, old_position + Position(0, 1));
    }

    {
      auto old_position = game_server->GetWorld().players.front()->position;
      mock_terminals.at(0)->AddInput((char)KeyCode::S);
      mock_terminals.at(0)->AddInput((char)KeyCode::S);
      mock_terminals.at(0)->AddInput((char)KeyCode::D);

      wait_a_few_iterations();
      auto new_position = game_server->GetWorld().players.front()->position;

      EXPECT_EQ(new_position, old_position + Position(1, 2));
    }

    {
      auto old_position = game_server->GetWorld().players.front()->position;
      mock_terminals.at(0)->AddInput((char)KeyCode::W);
      mock_terminals.at(0)->AddInput((char)KeyCode::A);

      wait_a_few_iterations();

      auto new_position = game_server->GetWorld().players.front()->position;

      EXPECT_EQ(new_position, old_position + Position(-1, -1));
    }

    mock_terminals.at(0)->AddInput((char)KeyCode::ESCAPE);
  });

  run_clients();
  input_thread.join();
  stop_server();

  expect_some_output_on_all();
}

TEST_F(GameNetworking, TwoPlayers) {
  communication_timeout = std::chrono::milliseconds(1000);

  create_server_and_client();
  create_new_client(Position(3, 4));

  std::thread input_thread([this] {
    wait_for_renderer();
    std::vector<std::wstring> outputs_before;
    for (auto& mock_terminal : mock_terminals) {
      outputs_before.push_back(mock_terminal->GetLastOutput());
    }

    wait_a_few_iterations();

    wait_for_renderer();
    // Check that the screen of all players has stayed the same
    for (size_t i = 0; i < mock_terminals.size(); ++i) {
      EXPECT_EQ(outputs_before.at(i), mock_terminals.at(i)->GetLastOutput());
    }

    auto first_player = game_server->GetWorld().GetPlayerById(1);
    auto second_player = game_server->GetWorld().GetPlayerById(2);

    Player old_first = *first_player;
    Player old_second = *second_player;

    // add input to a single player. This should change the world of all players.
    mock_terminals.at(0)->AddInput((char)KeyCode::S);
    mock_terminals.at(0)->AddInput((char)KeyCode::S);

    wait_a_few_iterations();
    wait_a_few_iterations();
    wait_a_few_iterations();

    EXPECT_FALSE(mock_terminals.at(0)->HasInput());
    EXPECT_FALSE(mock_terminals.at(1)->HasInput());

    EXPECT_EQ(old_first.position + Position(0, 2), first_player->position);
    EXPECT_EQ(old_second.position, second_player->position);

    for (const auto& game_client : game_clients) {
      EXPECT_EQ(first_player->position, game_client->GetWorld().GetPlayerById(1)->position);
      EXPECT_EQ(second_player->position, game_client->GetWorld().GetPlayerById(2)->position);
    }

    wait_for_renderer();
    // Check that the screen of all players has changed
    for (size_t i = 0; i < mock_terminals.size(); ++i) {
      EXPECT_NE(outputs_before.at(i), mock_terminals.at(i)->GetLastOutput());
    }

    wait_a_few_iterations();

    for (const auto& mock_terminal : mock_terminals) {
      mock_terminal->AddInput((char)KeyCode::ESCAPE);
    }
  });

  start_server();

  run_clients();

  input_thread.join();
  stop_server();

  expect_some_output_on_all();
}

TEST_F(GameNetworking, ManyPlayers) {
  // Necessary to prevent timeouts while creating the clients
  communication_timeout = std::chrono::milliseconds(1000);

  create_server_and_client();

  for (int i = 0; i < 100; ++i) {
    create_new_client(Position(i, 4));
  }

  for (auto& mock_terminal : mock_terminals) {
    add_inputs(*mock_terminal);
  }

  std::thread input_thread([this] {
    wait_a_few_iterations();

    for (const auto& mock_terminal : mock_terminals) {
      mock_terminal->AddInput((char)KeyCode::ESCAPE);
    }
  });

  start_server();

  run_clients();

  input_thread.join();
  stop_server();

  expect_some_output_on_all();
}
