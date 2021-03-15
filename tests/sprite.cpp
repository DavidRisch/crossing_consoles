#include <gtest/gtest.h>

#include "../src/game/visual/Sprite.h"
#include "../src/game/visual/symbols.h"

using namespace game::visual;
using namespace game::visual::symbols;

TEST(Sprite, GetLine1x1) {
  Sprite sprite(L"a");

  ASSERT_EQ(sprite.GetLine(0), L"a");
}

TEST(Sprite, GetLine2x1) {
  Sprite sprite(L"ab");

  ASSERT_EQ(sprite.GetLine(0), L"ab");
}

TEST(Sprite, GetLine1x2) {
  Sprite sprite(L"a\nb");

  ASSERT_EQ(sprite.GetLine(0), L"a");
  ASSERT_EQ(sprite.GetLine(1), L"b");
}

TEST(Sprite, GetLine2x2) {
  Sprite sprite(L"ab\ncd");

  ASSERT_EQ(sprite.GetLine(0), L"ab");
  ASSERT_EQ(sprite.GetLine(1), L"cd");
}

TEST(Sprite, GetLine3x3) {
  Sprite sprite(L"abc\ndef\nghi");

  ASSERT_EQ(sprite.GetLine(0), L"abc");
  ASSERT_EQ(sprite.GetLine(1), L"def");
  ASSERT_EQ(sprite.GetLine(2), L"ghi");
}

TEST(Sprite, GetLineUnicode) {
  Sprite sprite(std::wstring(2, box_drawings_double_horizontal) + L"\n"
                + std::wstring(2, box_drawings_double_vertical));

  ASSERT_EQ(sprite.GetLine(0), std::wstring(2, box_drawings_double_horizontal));
  ASSERT_EQ(sprite.GetLine(1), std::wstring(2, box_drawings_double_vertical));
}