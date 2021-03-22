#include <gtest/gtest.h>

#include <mutex>
#include <thread>

#include "../src/game/GameClient.h"
#include "../src/game/GameServer.h"
#include "../src/game/terminal/MockTerminal.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::visual;
using namespace game::terminal;
using namespace game::terminal::colors;

class GameNetworking : public ::testing::Test {
 public:
  GameNetworking() = default;

  std::shared_ptr<GameClient> game_client;
  std::shared_ptr<GameServer> game_server;

  std::shared_ptr<MockTerminal> mock_terminal;

  bool keep_server_running;
  std::shared_ptr<std::thread> server_thread;

  void start_server() {
    assert(server_thread == nullptr);

    keep_server_running = true;

    server_thread = std::make_shared<std::thread>([this] {
      while (keep_server_running) {
        game_server->RunIteration();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // TODO: shorter sleep
      }
    });
  }

  void stop_server() {
    assert(server_thread != nullptr);

    keep_server_running = false;
    server_thread->join();
    server_thread.reset();
  }

  void create_server_and_client() {
    game_server = std::make_shared<GameServer>(coordinate_size_t(20, 15));

    Player player("user", Position(0, 0));
    mock_terminal = std::make_shared<MockTerminal>();

    start_server();
    game_client = std::make_shared<GameClient>(player, mock_terminal, coordinate_size_t(1, 1), true);
    stop_server();
  }

  void expect_terminal_not_empty() {
    bool empty = true;
    for (const auto& i_lines : mock_terminal->GetLastOutput()) {
      for (const auto& i_characters : i_lines) {
        if (i_characters != ColoredChar(L' ', WHITE, BLACK)) {
          empty = false;
        }
      }
    }

    ASSERT_FALSE(empty);
  }
};

TEST_F(GameNetworking, NoAction) {
  create_server_and_client();

  mock_terminal->AddInput((char)KeyCode::ESCAPE);

  start_server();

  game_client->Run();

  stop_server();

  expect_terminal_not_empty();
}

TEST_F(GameNetworking, Actions) {
  create_server_and_client();

  mock_terminal->AddInput((char)KeyCode::W);
  mock_terminal->AddInput((char)KeyCode::A);
  mock_terminal->AddInput((char)KeyCode::S);
  mock_terminal->AddInput((char)KeyCode::D);
  mock_terminal->AddInput((char)KeyCode::ESCAPE);

  start_server();

  game_client->Run();

  stop_server();

  expect_terminal_not_empty();
}
