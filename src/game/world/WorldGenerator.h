#ifndef CROSSING_CONSOLES_WORLDGENERATOR_H
#define CROSSING_CONSOLES_WORLDGENERATOR_H

#include <memory>

#include "HeightMap.h"
#include "World.h"

namespace game::world {

class WorldGenerator {
 public:
  std::shared_ptr<World> GenerateWorld(const common::coordinate_size_t& world_size);
  std::shared_ptr<World> GenerateEmptyWorld(const common::coordinate_size_t& world_size);

  WorldGenerator();
  explicit WorldGenerator(int seed);

 private:
  int seed;
  common::coordinate_size_t size = common::coordinate_size_t(0, 0);

  HeightMap height_map;
  const int building_count_min = 5;
  const int building_count_max = 15;
  const common::coordinate_size_t building_size_min = common::coordinate_size_t(7, 5);
  const common::coordinate_size_t building_size_max = common::coordinate_size_t(12, 10);

  const double persistence = 10;
  const int octaves = 4;

  [[nodiscard]] double GenerateNoise(int i, int x, int y) const;
  double SmoothNoise(int i, int x, int y);
  static double Interpolate(double a, double b, double x);
  double InterpolateNoise(int i, double x, double y);
  double PerlinNoise(double x, double y);
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_WORLDGENERATOR_H
