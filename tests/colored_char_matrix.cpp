#include <gtest/gtest.h>

#include "../src/game/visual/ColoredCharMatrix.h"
#include "../src/game/visual/symbols.h"

using namespace game::common;
using namespace game::visual;
using namespace game::visual::symbols;
using namespace game::terminal::colors;

TEST(ColoredCharMatrix, EmptyMatrix) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(2, 2));

  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
    }
  }
}

TEST(ColoredCharMatrix, AppendChar) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.AppendChar(L'a');
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_characters == &matrix[0][0]) {
        EXPECT_EQ(i_characters, ColoredChar(L'a', WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}

TEST(ColoredCharMatrix, AppendCharRepeated) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(2, 2));

  colored_string_matrix.AppendChar(L'a');
  colored_string_matrix.AppendChar(L'b');
  colored_string_matrix.AppendChar(L'c');
  colored_string_matrix.AppendChar(L'd');
  colored_string_matrix.AppendChar(L'e');
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  EXPECT_EQ(matrix[0][0], ColoredChar(L'a', WHITE, BLACK));
  EXPECT_EQ(matrix[0][1], ColoredChar(L'b', WHITE, BLACK));
  EXPECT_EQ(matrix[1][0], ColoredChar(L'c', WHITE, BLACK));
  EXPECT_EQ(matrix[1][1], ColoredChar(L'd', WHITE, BLACK));
}

TEST(ColoredCharMatrix, SetChar) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(L'a', Position(0, 0));
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_characters == &matrix[0][0]) {
        EXPECT_EQ(i_characters, ColoredChar(L'a', WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}

TEST(ColoredCharMatrix, SetCharRepeated) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetChar(L'a', Position(0, 0));
  colored_string_matrix.SetChar(L'b', Position(1, 0));
  colored_string_matrix.SetChar(L'c', Position(2, 0));
  colored_string_matrix.SetChar(L'd', Position(4, 0));
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_characters == &matrix[0][0]) {
        EXPECT_EQ(i_characters, ColoredChar(L'a', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][1]) {
        EXPECT_EQ(i_characters, ColoredChar(L'b', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][2]) {
        EXPECT_EQ(i_characters, ColoredChar(L'c', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][4]) {
        EXPECT_EQ(i_characters, ColoredChar(L'd', WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}

TEST(ColoredCharMatrix, AppendUnicodeChar) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.AppendChar(full_block);
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_characters == &matrix[0][0]) {
        EXPECT_EQ(i_characters, ColoredChar(full_block, WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}

TEST(ColoredCharMatrix, AppendString) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.AppendString(L"abcd");
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_characters == &matrix[0][0]) {
        EXPECT_EQ(i_characters, ColoredChar(L'a', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][1]) {
        EXPECT_EQ(i_characters, ColoredChar(L'b', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][2]) {
        EXPECT_EQ(i_characters, ColoredChar(L'c', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][3]) {
        EXPECT_EQ(i_characters, ColoredChar(L'd', WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}

TEST(ColoredCharMatrix, SetString) {
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  colored_string_matrix.SetString(L"abcd", Position(1, 0));
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_characters == &matrix[0][1]) {
        EXPECT_EQ(i_characters, ColoredChar(L'a', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][2]) {
        EXPECT_EQ(i_characters, ColoredChar(L'b', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][3]) {
        EXPECT_EQ(i_characters, ColoredChar(L'c', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][4]) {
        EXPECT_EQ(i_characters, ColoredChar(L'd', WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}

TEST(ColoredCharMatrix, InsertMatrix) {
  ColoredCharMatrix sprite_1(coordinate_size_t(2, 2));
  ColoredCharMatrix sprite_2(coordinate_size_t(2, 2));
  ColoredCharMatrix colored_string_matrix(coordinate_size_t(10, 10));

  sprite_1.AppendString(L"abcd");
  sprite_2.AppendString(L"efgh");
  colored_string_matrix.InsertMatrix(sprite_1);
  colored_string_matrix.InsertMatrix(sprite_2, Position(7, 7));
  const std::vector<std::vector<ColoredChar>>& matrix = colored_string_matrix.GetMatrix();

  for (const auto& i_lines : matrix) {
    for (const auto& i_characters : i_lines) {
      if (&i_characters == &matrix[0][0]) {
        EXPECT_EQ(i_characters, ColoredChar(L'a', WHITE, BLACK));
      } else if (&i_characters == &matrix[0][1]) {
        EXPECT_EQ(i_characters, ColoredChar(L'b', WHITE, BLACK));
      } else if (&i_characters == &matrix[1][0]) {
        EXPECT_EQ(i_characters, ColoredChar(L'c', WHITE, BLACK));
      } else if (&i_characters == &matrix[1][1]) {
        EXPECT_EQ(i_characters, ColoredChar(L'd', WHITE, BLACK));
      } else if (&i_characters == &matrix[7][7]) {
        EXPECT_EQ(i_characters, ColoredChar(L'e', WHITE, BLACK));
      } else if (&i_characters == &matrix[7][8]) {
        EXPECT_EQ(i_characters, ColoredChar(L'f', WHITE, BLACK));
      } else if (&i_characters == &matrix[8][7]) {
        EXPECT_EQ(i_characters, ColoredChar(L'g', WHITE, BLACK));
      } else if (&i_characters == &matrix[8][8]) {
        EXPECT_EQ(i_characters, ColoredChar(L'h', WHITE, BLACK));
      } else {
        EXPECT_EQ(i_characters, ColoredChar(L' ', WHITE, BLACK));
      }
    }
  }
}