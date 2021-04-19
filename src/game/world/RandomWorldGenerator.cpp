#include "RandomWorldGenerator.h"

#include <ctime>

using namespace game;
using namespace game::common;
using namespace game::world;

RandomWorldGenerator::RandomWorldGenerator() {
  std::random_device rd;
  int random_seed = rd();

  Construct(random_seed);
}

RandomWorldGenerator::RandomWorldGenerator(int seed) {
  Construct(seed);
}

void RandomWorldGenerator::Construct(int new_seed) {
  seed = new_seed;
  random.seed(seed);

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

std::shared_ptr<World> RandomWorldGenerator::GenerateWorld(const coordinate_size_t& world_size) {
  size = world_size;
  world = std::make_shared<World>(size);

  GenerateHeight();

  if (size.IsGreater(building_size_max)) {
    GenerateBuildings();
  }

  return world;
}

void RandomWorldGenerator::GenerateHeight() {
  // generate landscape based on heightmap
  int empty_blocks = 0;
  double height;
  for (int x = 0; x < size.x; x++) {
    for (int y = 0; y < size.y; y++) {
      height = PerlinNoise(x, y);

      int distance_to_closest_edge = std::min({x, size.x - x - 1, y, size.y - y - 1});

      // smooth out walls next to world edges
      if (distance_to_closest_edge < smoothing_distance) {
        double factor = distance_to_closest_edge / (double)smoothing_distance;
        height *= factor;
      }

      BlockType type = height_map.GetType((int)height);
      if (type == BlockType::EMPTY_BLOCK) {
        empty_blocks++;
      }
      if (type >= BlockType::WALL_START && type <= BlockType::WALL_END) {
        world->AddWall(Position(x, y), type);
      }
    }
  }
  building_count = static_cast<int>((empty_blocks * building_rate) / (building_size_max.x * building_size_max.y));
}

void RandomWorldGenerator::GenerateBuildings() {
  std::uniform_int_distribution<int> position_x(0, size.x - building_size_max.x - 1);
  std::uniform_int_distribution<int> position_y(0, size.y - building_size_max.y - 1);
  std::uniform_int_distribution<int> size_x(building_size_min.x, building_size_max.x);
  std::uniform_int_distribution<int> size_y(building_size_min.y, building_size_max.y);

  // generate buildings
  for (int n = building_count; n > 0; n--) {
    // calculate start and end position of building
    Position start(position_x(random), position_y(random));
    Position end = start + Position(size_x(random), size_y(random));

    // check if area is empty
    if (world->IsBlocked(start) || world->IsBlocked(end) || world->IsBlocked(Position(start.x, end.y - 1)) ||
        world->IsBlocked(Position(end.x - 1, start.y))) {
      n++;
      continue;
    }

    for (int y = start.y; y < end.y; y++) {
      for (int x = start.x; x < end.x; x++) {
        if (x == start.x || x == end.x - 1 || y == start.y || y == end.y - 1) {
          Position current_position(x, y);
          // place edges of building
          world->AddWall(Position(x, y), BlockType::WALL_BRICK);
        } else {
          // hollow inside of building out
          world->RemoveWall(Position(x, y));
        }
      }
    }

    GenerateDoors(start, end);
  }
}

void RandomWorldGenerator::GenerateDoors(const Position& start, const Position& end) {
  std::uniform_int_distribution<int> building_x(start.x + 1, end.x - 2);
  std::uniform_int_distribution<int> building_y(start.y + 1, end.y - 2);
  std::uniform_int_distribution<int> door_chance(0, 1);

  // place doors randomly
  int door_count = 0;
  Position door_position(0, 0);
  for (int i = 0; i < 4; i++) {
    if (door_chance(random) == 0) {
      switch (i) {
        case 0:
          // up
          door_position.Set(building_x(random), start.y);
          if (world->IsBlocked(door_position + Position(0, -1))) {
            continue;
          }
          break;
        case 1:
          // down
          door_position.Set(building_x(random), end.y - 1);
          if (world->IsBlocked(door_position + Position(0, 1))) {
            continue;
          }
          break;
        case 2:
          // left
          door_position.Set(start.x, building_y(random));
          if (world->IsBlocked(door_position + Position(-1, 0))) {
            continue;
          }
          break;
        case 3:
          // right
          door_position.Set(end.x - 1, building_y(random));
          if (world->IsBlocked(door_position + Position(1, 0))) {
            continue;
          }
          break;
      }
      world->RemoveWall(door_position);
      door_count++;
    }

    // repeat if no door has been placed
    if (i == 3 && door_count == 0) {
      i=-1;
    }
  }
}

double RandomWorldGenerator::GenerateNoise(int i, int x, int y) {
  std::uniform_real_distribution<double> noise_distribution = std::uniform_real_distribution<double>(-1.0, 1.0);
  random_perlin_noise.seed(seed + (i + 1) * (x + (y * size.x)));

  return noise_distribution(random_perlin_noise);
}

double RandomWorldGenerator::SmoothNoise(int i, int x, int y) {
  double corners = (GenerateNoise(i, x - 1, y - 1) + GenerateNoise(i, x + 1, y - 1) + GenerateNoise(i, x - 1, y + 1) +
                    GenerateNoise(i, x + 1, y + 1)) /
                   16;
  double sides = (GenerateNoise(i, x - 1, y) + GenerateNoise(i, x + 1, y) + GenerateNoise(i, x, y - 1) +
                  GenerateNoise(i, x, y + 1)) /
                 8;
  double center = GenerateNoise(i, x, y) / 4;

  return corners + sides + center;
}

double RandomWorldGenerator::Interpolate(double a, double b, double x) {
  double ft = x * M_PI;
  double f = (1 - cos(ft)) * 0.5;
  return a * (1 - f) + b * f;
}

double RandomWorldGenerator::InterpolateNoise(int i, double x, double y) {
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

double RandomWorldGenerator::PerlinNoise(double x, double y) {
  double total = 0;
  double frequency = 0;
  double amplitude;

  for (int i = 0; i < octaves; i++) {
    frequency = pow(2, i);
    amplitude = pow(persistence, i);
    total += InterpolateNoise(i, x / frequency, y / frequency) * amplitude;
  }
  return total / frequency * height_factor;
}
