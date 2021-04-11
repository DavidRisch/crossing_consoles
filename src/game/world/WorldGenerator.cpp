#include "WorldGenerator.h"

#include <ctime>
#include <random>

using namespace game;
using namespace game::common;
using namespace game::world;

WorldGenerator::WorldGenerator() {
  std::random_device rd;
  seed = rd();

  height_map.SetType(10, BlockType::WALL_WATER);
  height_map.SetType(10, BlockType::EMPTY_BLOCK);
  height_map.SetType(28, BlockType::WALL_ROCK_LIGHT);
  height_map.SetType(29, BlockType::WALL_ROCK_MEDIUM);
  height_map.SetType(30, BlockType::WALL_ROCK_HEAVY);
  height_map.SetType(31, BlockType::WALL_ROCK_FULL);
  height_map.SetType(58, BlockType::WALL_SNOW_LIGHT);
  height_map.SetType(59, BlockType::WALL_SNOW_MEDIUM);
  height_map.SetType(60, BlockType::WALL_SNOW_HEAVY);
  height_map.SetType(61, BlockType::WALL_SNOW_FULL);
}

WorldGenerator::WorldGenerator(int seed)
    : seed(seed) {
}

std::shared_ptr<World> WorldGenerator::GenerateWorld(const coordinate_size_t& world_size) {
  size = world_size;
  auto world = std::make_shared<World>(world_size);

  // generate landscape based on heightmap
  double height;
  for (int x = 0; x < world_size.x; x++) {
    for (int y = 0; y < world_size.y; y++) {
      height = PerlinNoise(x, y) * 2;

      BlockType type = height_map.GetType((int)height);
      if (type >= BlockType::WALL_START && type <= BlockType::WALL_END) {
        world->AddWall(Position(x, y), type);
      }
    }
  }

  if (world_size.IsGreater(building_size_max)) {
    std::mt19937 random(seed);
    std::uniform_int_distribution<int> count(building_count_min, building_count_max);
    std::uniform_int_distribution<int> position_x(0, size.x - building_size_max.x - 1);
    std::uniform_int_distribution<int> position_y(0, size.y - building_size_max.y - 1);
    std::uniform_int_distribution<int> size_x(building_size_min.x, building_size_max.x);
    std::uniform_int_distribution<int> size_y(building_size_min.y, building_size_max.y);

    // generate buildings
    for (int n = count(random); n > 0; n--) {
      // calculate start and end position of building
      Position start(position_x(random), position_y(random));
      Position end = start + Position(size_x(random), size_y(random));

      // check if area is spare
      if (world->IsBlocked(start) || world->IsBlocked(end) || world->IsBlocked(Position(start.x, end.y - 1)) ||
          world->IsBlocked(Position(end.x - 1, start.y))) {
        n++;
        continue;
      }

      for (int y = start.y; y < end.y; y++) {
        for (int x = start.x; x < end.x; x++) {
          if (x == start.x || x == end.x - 1 || y == start.y || y == end.y - 1) {
            Position current_position(x, y);
            // place corners of building
            world->AddWall(Position(x, y), BlockType::WALL_BRICK);
          } else {
            // hollow inside of building out
            world->RemoveWall(Position(x, y));
          }
        }
      }

      std::uniform_int_distribution<int> building_x(start.x + 1, end.x - 2);
      std::uniform_int_distribution<int> building_y(start.y + 1, end.y - 2);
      std::uniform_int_distribution<int> door_chance(0, 1);

      // place doors randomly
      int door_count = 0;
      for (int i = 0; i < 4; i++) {
        if (door_chance(random) == 0) {
          door_count++;
          switch (i) {
            case 0:
              world->RemoveWall(Position(building_x(random), start.y));
              break;
            case 1:
              world->RemoveWall(Position(building_x(random), end.y - 1));
              break;
            case 2:
              world->RemoveWall(Position(start.x, building_y(random)));
              break;
            case 3:
              world->RemoveWall(Position(end.x - 1, building_y(random)));
              break;
          }
        }

        // repeat if no door has been placed
        if (i == 3 && door_count == 0) {
          i--;
        }
      }
    }
  }

  return world;
}

double WorldGenerator::GenerateNoise(int i, int x, int y) const {
  std::mt19937 random(seed + (i + 1) * (x + (y * size.x)));
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
