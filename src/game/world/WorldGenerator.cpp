#include "WorldGenerator.h"

#include <ctime>
#include <random>

using namespace game;
using namespace game::common;
using namespace game::world;

WorldGenerator::WorldGenerator() {
  std::random_device rd;
  seed = rd();

  height_map.SetType(0, BlockType::WALL_WATER);
  height_map.SetType(1000, BlockType::EMPTY_BLOCK);
  height_map.SetType(3000, BlockType::WALL_ROCK);
  height_map.SetType(6000, BlockType::WALL_SNOW);
}

WorldGenerator::WorldGenerator(int seed)
    : seed(seed) {
}

std::shared_ptr<World> WorldGenerator::GenerateWorld(const coordinate_size_t& world_size) {
  auto world = std::make_shared<World>(world_size);

  double height;

  for (int x = 0; x < world_size.x; x++) {
    for (int y = 0; y < world_size.y; y++) {
      height = PerlinNoise(x, y);

      BlockType type = height_map.GetType((int)height);
      if (type >= BlockType::WALL_START && type <= BlockType::WALL_END) {
        world->AddWall(Position(x, y), type);
      }
    }
  }

  return world;
}

double WorldGenerator::GenerateNoise(int i, int x, int y) const {
  std::mt19937 random(seed + i + x * 10 + y * 100);
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  return distribution(random);
}

double WorldGenerator::SmoothNoise(int i, int x, int y) {
  double corners = (GenerateNoise(i, x - 1, y - 1) + GenerateNoise(i, x + 1, y - 1) + GenerateNoise(i, x - 1, y + 1) +
                    GenerateNoise(i, x + 1, y + 1)) /
                   16;
  double sides = (GenerateNoise(i, x - 1, y) + GenerateNoise(i, x + 1, y) + GenerateNoise(i, x, y - 1) +
                  GenerateNoise(i, x, y + 1)) /
                 8;
  double center = GenerateNoise(i, x, y) / 4;

  return corners + sides + center;
}

double WorldGenerator::Interpolate(double a, double b, double x) {
  double ft = x * M_PI;
  double f = (1 - cos(ft)) * 0.5;
  return a * (1 - f) + b * f;
}

double WorldGenerator::InterpolateNoise(int i, double x, double y) {
  int integer_x = (int)x;
  double fractional_x = x - (float)integer_x;
  int integer_y = (int)y;
  double fractional_y = y - (float)integer_y;

  double v1 = SmoothNoise(i, integer_x, integer_y);
  double v2 = SmoothNoise(i, integer_x + 1, integer_y);
  double v3 = SmoothNoise(i, integer_x, integer_y + 1);
  double v4 = SmoothNoise(i, integer_x + 1, integer_y + 1);
  double i1 = Interpolate(v1, v2, fractional_x);
  double i2 = Interpolate(v3, v4, fractional_x);

  return Interpolate(i1, i2, fractional_y);
}

double WorldGenerator::PerlinNoise(double x, double y) {
  double total = 0;
  double frequency;
  double amplitude;

  for (int i = 0; i < octaves; i++) {
    frequency = pow(2, i);
    amplitude = pow(persistence, i);
    total += InterpolateNoise(i, x / frequency, y / frequency) * amplitude;
  }
  return total / frequency;
}
