#include <gtest/gtest.h>

#include "../src/game/visual/ColoredCharMatrix.h"
#include "../src/game/visual/symbols.h"

using namespace game::visual;
using namespace game::visual::symbols;

TEST(ColoredStringMatrix, EmptyMatrix) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(2, 2));

  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
    }
  }
}

TEST(ColoredStringMatrix, SetChar) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(L'a');
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_lines == &matrix.front() && &i_characters == &i_lines.front()) {
        EXPECT_EQ(i_characters, ColoredChar(L'a', WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}

TEST(ColoredStringMatrix, SetCharRepeated) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(2, 2));

  colored_string_matrix.SetChar(L'a');
  colored_string_matrix.SetChar(L'b');
  colored_string_matrix.SetChar(L'c');
  colored_string_matrix.SetChar(L'd');
  colored_string_matrix.SetChar(L'e');
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  EXPECT_EQ(matrix[0][0], ColoredChar(L'a', WHITE, BLACK));
  EXPECT_EQ(matrix[0][1], ColoredChar(L'b', WHITE, BLACK));
  EXPECT_EQ(matrix[1][0], ColoredChar(L'c', WHITE, BLACK));
  EXPECT_EQ(matrix[1][1], ColoredChar(L'd', WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetCharWithPosition) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(L'a', Position(0, 0));
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_lines == &matrix.front() && &i_characters == &i_lines.front()) {
        EXPECT_EQ(i_characters, ColoredChar(L'a', WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}

TEST(ColoredStringMatrix, SetCharWithPositionRepeated) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(L'a', Position(0, 0));
  colored_string_matrix.SetChar(L'b', Position(1, 0));
  colored_string_matrix.SetChar(L'c', Position(2, 0));
  colored_string_matrix.SetChar(L'd', Position(4, 0));
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  EXPECT_EQ(matrix[0][0], ColoredChar(L'a', WHITE, BLACK));
  EXPECT_EQ(matrix[0][1], ColoredChar(L'b', WHITE, BLACK));
  EXPECT_EQ(matrix[0][2], ColoredChar(L'c', WHITE, BLACK));
  EXPECT_EQ(matrix[0][3], ColoredChar(L' ', WHITE, BLACK));
  EXPECT_EQ(matrix[0][4], ColoredChar(L'd', WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetUnicodeChar) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(full_block);
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_lines == &matrix.front() && &i_characters == &i_lines.front()) {
        EXPECT_EQ(i_characters, ColoredChar(full_block, WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}

TEST(ColoredStringMatrix, SetString) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetString(L"abcd");
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  EXPECT_EQ(matrix[0][0], ColoredChar(L'a', WHITE, BLACK));
  EXPECT_EQ(matrix[0][1], ColoredChar(L'b', WHITE, BLACK));
  EXPECT_EQ(matrix[0][2], ColoredChar(L'c', WHITE, BLACK));
  EXPECT_EQ(matrix[0][3], ColoredChar(L'd', WHITE, BLACK));
}

TEST(ColoredStringMatrix, SetStringWithPosition) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetString(L"abcd", Position(1, 0));
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  EXPECT_EQ(matrix[0][0], ColoredChar(L' ', WHITE, BLACK));
  EXPECT_EQ(matrix[0][1], ColoredChar(L'a', WHITE, BLACK));
  EXPECT_EQ(matrix[0][2], ColoredChar(L'b', WHITE, BLACK));
  EXPECT_EQ(matrix[0][3], ColoredChar(L'c', WHITE, BLACK));
  EXPECT_EQ(matrix[0][4], ColoredChar(L'd', WHITE, BLACK));
}
