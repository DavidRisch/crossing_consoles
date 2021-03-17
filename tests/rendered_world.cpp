#include <gtest/gtest.h>

#include "../src/game/visual/RenderedWorld.h"
#include "../src/game/visual/symbols.h"

using namespace game::visual;
using namespace game::visual::symbols;

TEST(RenderedWorld, Empty) {
  RenderedWorld rendered_world(coordinate_size_t(2, 2));

  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L' ', WHITE, BLACK));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L' ', WHITE, BLACK));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L' ', WHITE, BLACK));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L' ', WHITE, BLACK));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'\0', WHITE, BLACK));
}

TEST(RenderedWorld, SetChar) {
  RenderedWorld rendered_world(coordinate_size_t(10, 10));

  rendered_world.SetChar(L'a', Position(0, 0));

  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'a', WHITE, BLACK));
}

TEST(RenderedWorld, SetUnicodeChar) {
  RenderedWorld rendered_world(coordinate_size_t(10, 10));

  rendered_world.SetChar(full_block, Position(0, 0));

  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(full_block, WHITE, BLACK));
}

TEST(RenderedWorld, SetChars) {
  RenderedWorld rendered_world(coordinate_size_t(10, 10));

  rendered_world.SetChar(L'a', Position(0, 0), WHITE, BLACK);
  rendered_world.SetChar(L'b', Position(1, 0), RED, BLUE);
  rendered_world.SetChar(L'c', Position(2, 0), GREEN, YELLOW);
  rendered_world.SetChar(L'd', Position(4, 0), BLACK, WHITE);

  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'a', WHITE, BLACK));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'b', RED, BLUE));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'c', GREEN, YELLOW));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L' ', WHITE, BLACK));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'd', BLACK, WHITE));
}

TEST(RenderedWorld, SetString) {
  RenderedWorld rendered_world(coordinate_size_t(10, 10));

  rendered_world.SetString(L"abcd", Position(1, 0), BLACK, WHITE);

  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L' ', WHITE, BLACK));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'a', BLACK, WHITE));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'b', BLACK, WHITE));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'c', BLACK, WHITE));
  EXPECT_EQ(rendered_world.GetChar(), std::make_tuple(L'd', BLACK, WHITE));
}
