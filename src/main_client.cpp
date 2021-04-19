#include <cassert>
#include <csignal>
#include <iostream>

#include "game/GameClient.h"
#include "game/common/Color.h"
#include "game/terminal/RealTerminal.h"
#include "game/world/Player.h"

using namespace game;
using namespace game::common;
using namespace game::world;
using namespace game::terminal;

GameClient *GAME_CLIENT;

void SignalHandler(int signal) {
  if (signal != SIGINT) {
    throw std::runtime_error("Unexpected signal caught");
  }
  assert(GAME_CLIENT != nullptr);
  GAME_CLIENT->StartExit();
}

void SetupSignalHandler(GameClient *game_client) {
  GAME_CLIENT = game_client;

#ifdef _WIN32
  // https://stackoverflow.com/questions/32389905/sigaction-and-porting-linux-code-to-windows
  signal(SIGINT, SignalHandler);
#else
  struct sigaction sigIntHandler {};
  sigIntHandler.sa_handler = SignalHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, nullptr);
#endif
}

int main(int argc, char *argv[]) {
  // default arguments
  std::string name = "?";
  Color color(200, 200, 200);

  if (argc < 1 || argc > 3) {
    throw std::runtime_error("Invalid number of command line arguments.");
  }
  if (argc >= 2) {
    // set player name
    name = argv[1];
    if (name.size() < GameDefinition::name_length_min) {
      throw std::runtime_error("Name '" + name + "' is too short, must be at least " +
                               std::to_string(GameDefinition::name_length_min) + " chars long.");
    }
    if (name.size() > GameDefinition::name_length_max) {
      throw std::runtime_error("Name '" + name + "' is too long, must be at most " +
                               std::to_string(GameDefinition::name_length_max) + " chars long.");
    }

    if (std::all_of(name.begin(), name.end(), isspace)) {
      throw std::runtime_error("The name is not allowed to consist of whitespaces only. Please change it.");
    }

    if (std::any_of(name.begin(), name.end(), [](char c) { return !isprint(c); })) {
      throw std::runtime_error("The name contains non-printable characters. Please change it.");
    }
  }
  if (argc >= 3) {
    std::string hex_color = argv[2];

    if (hex_color.size() != 6) {
      throw std::runtime_error("Invalid color format. Example of correct color: 'ff0000'.");
    }

    // decode hexadecimal color values
    color.red = std::stoul(hex_color.substr(0, 2), nullptr, 16);
    color.green = std::stoul(hex_color.substr(2, 2), nullptr, 16);
    color.blue = std::stoul(hex_color.substr(4, 2), nullptr, 16);

    // see https://en.wikipedia.org/wiki/YUV for the calculation of the luminance (Y) from the RGB values
    double luminance = color.red * 0.299 + color.green * 0.587 + color.blue * 0.114;
    if (luminance < 70) {
      throw std::runtime_error("The chosen color is too dark, please choose a brighter one.");
    }
  }

  auto player = std::make_shared<Player>(name, color, Position(0, 0), GameDefinition::Direction::NORTH, 999);
  auto terminal = std::make_shared<RealTerminal>();
  GameClient game_client(player, terminal, coordinate_size_t(30, 30), true);

  SetupSignalHandler(&game_client);

  game_client.Run();

  RealTerminal::Restore();
}
