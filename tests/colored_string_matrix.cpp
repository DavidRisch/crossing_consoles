#include <gtest/gtest.h>

#include "../src/game/visual/ColoredCharMatrix.h"
#include "../src/game/visual/symbols.h"

using namespace game::visual;
using namespace game::visual::symbols;

TEST(ColoredStringMatrix, GetChar) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L' ', WHITE, BLACK));
}

TEST(ColoredStringMatrix, GetCharRepeated) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(2, 2));

  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L' ', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L' ', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'\n', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L' ', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L' ', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'\n', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'\0', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'\0', WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetChar) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(L'a');

  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'a', WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetCharRepeated) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(2, 2));

  colored_string_matrix.SetChar(L'a');
  colored_string_matrix.SetChar(L'b');
  colored_string_matrix.SetChar(L'c');
  colored_string_matrix.SetChar(L'd');
  colored_string_matrix.SetChar(L'e');

  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'a', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'b', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'\n', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'c', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'd', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'\n', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'\0', WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetCharWithPosition) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(L'a', Position(0, 0));

  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'a', WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetCharWithPositionRepeated) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(L'a', Position(0, 0));
  colored_string_matrix.SetChar(L'b', Position(1, 0));
  colored_string_matrix.SetChar(L'c', Position(2, 0));
  colored_string_matrix.SetChar(L'd', Position(4, 0));

  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'a', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'b', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'c', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L' ', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'd', WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetUnicodeChar) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(full_block);

  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(full_block, WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetString) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetString(L"abcd");

  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'a', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'b', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'c', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'd', WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetStringWithPosition) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetString(L"abcd", Position(1, 0));

  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L' ', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'a', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'b', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'c', WHITE, BLACK));
  EXPECT_EQ(colored_string_matrix.GetChar(), ColoredChar(L'd', WHITE, BLACK));
}
