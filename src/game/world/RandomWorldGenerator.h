#ifndef CROSSING_CONSOLES_RANDOMWORLDGENERATOR_H
#define CROSSING_CONSOLES_RANDOMWORLDGENERATOR_H

#include <memory>
#include <random>

#include "HeightMap.h"
#include "IWorldGenerator.h"
#include "World.h"

namespace game::world {

/**
 * \brief Used for generating random worlds.
 */
class RandomWorldGenerator : public IWorldGenerator {
 public:
  /**
   * \brief Generates a random world.
   */
  std::shared_ptr<World> GenerateWorld(const common::coordinate_size_t& world_size) override;

  RandomWorldGenerator();
  explicit RandomWorldGenerator(int seed);

 private:
  /**
   * \brief Set seed and heightmap.
   */
  void Construct(int seed);

  std::shared_ptr<World> world;
  common::coordinate_size_t size = common::coordinate_size_t(0, 0);

  int seed{};
  HeightMap height_map;
  const int smoothing_distance = 5;

  // building generation parameters
  int building_count{};
  double building_rate = 0.1;
  const common::coordinate_size_t building_size_min = common::coordinate_size_t(7, 5);
  const common::coordinate_size_t building_size_max = common::coordinate_size_t(12, 10);

  // perlin noise parameters
  const double persistence = 10;
  const int octaves = 4;
  const int height_factor = 2;

  /**
   * \brief Generate landscape from heightmap.
   */
  void GenerateHeight();
  /**
   * \brief Generate empty buildings.
   */
  void GenerateBuildings();
  /**
   * \brief Generate doors in buildings.
   * \details Used in GenerateBuildings().
   */
  void GenerateDoors(const common::Position& start, const common::Position& end);

  /**
   * \brief Generate random noise for coordinate pair and corresponding octave.
   */
  [[nodiscard]] double GenerateNoise(int i, int x, int y);
  /**
   * \brief Smooth out noise function by looking at neighbouring values.
   */
  double SmoothNoise(int i, int x, int y);
  /**
   * \brief Smooth out noise function by interpolating.
   */
  static double Interpolate(double a, double b, double x);
  /**
   * \brief Combining interpolation and smoothing.
   */
  double InterpolateNoise(int i, double x, double y);
  /**
   * \brief Generate 2D Perlin noise.
   * \details See https://web.archive.org/web/20160530124230/http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
   * for reference.
   */
  double PerlinNoise(double x, double y);

  std::mt19937 random;
  std::mt19937 random_perlin_noise;
};

}  // namespace game::world

#endif  // CROSSING_CONSOLES_RANDOMWORLDGENERATOR_H
