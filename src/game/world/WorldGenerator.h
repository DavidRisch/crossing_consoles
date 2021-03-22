#ifndef CROSSING_CONSOLES_WORLDGENERATOR_H
#define CROSSING_CONSOLES_WORLDGENERATOR_H

#include <memory>

#include "World.h"

namespace game::world {

class WorldGenerator {
 public:
  std::shared_ptr<World> GenerateWorld(const common::coordinate_size_t& world_size);

  WorldGenerator();
  explicit WorldGenerator(int seed);

 private:
  int seed;
  const double persistence = 50;
  const int octaves = 4;
  [[nodiscard]] double GenerateNoise(int i, int x, int y) const;
  double SmoothNoise(int i, int x, int y);
  static double Interpolate(double a, double b, double x);
  double InterpolateNoise(int i, double x, double y);
  double PerlinNoise(double x, double y);
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_WORLDGENERATOR_H
