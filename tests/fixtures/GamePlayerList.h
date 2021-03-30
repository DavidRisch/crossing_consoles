#ifndef CROSSING_CONSOLES_GAME_PLAYERS_H
#define CROSSING_CONSOLES_GAME_PLAYERS_H

#include <gtest/gtest.h>

#include <codecvt>
#include <locale>

class GamePlayerList : public ::testing::Test {
 public:
  static std::wstring ToWstring(const std::string& narrow_string) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    return (converter.from_bytes(narrow_string));
  }
};

#endif  // CROSSING_CONSOLES_GAME_PLAYERS_H
