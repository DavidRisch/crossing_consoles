#include "SpriteMap.h"

using namespace game;
using namespace game::visual;
using namespace game::world;

SpriteMap::SpriteMap(common::coordinate_size_t block_size)
    : block_size(block_size) {
  SetSprite(BlockType::EMPTY_BLOCK, ColoredCharMatrix(block_size));
}

void SpriteMap::SetSprite(BlockType type, const ColoredCharMatrix& sprite) {
  if (sprite.GetSize() == block_size) {
    auto const result = type_sprites.insert(std::pair<BlockType, ColoredCharMatrix>(type, sprite));
    if (!result.second) {
      result.first->second = sprite;
    }
  } else {
    throw SpriteWrongSizeException();
  }
}

const ColoredCharMatrix& SpriteMap::GetSprite(BlockType type) const {
  auto result = type_sprites.find(type);
  if (result != type_sprites.end()) {
    return type_sprites.at(type);
  } else {
    throw BlockTypeNotFoundException();
  }
}
