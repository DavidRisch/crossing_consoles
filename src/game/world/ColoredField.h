#ifndef CROSSING_CONSOLES_COLOREDFIELD_H
#define CROSSING_CONSOLES_COLOREDFIELD_H

#include <cstdint>

#include "../common/Position.h"

namespace game::world {

/**
 * \brief Represent fields in the `World` on which damage was caused.
 */
class ColoredField : public networking::ISerializable {
 public:
  /**
   * \brief Construct new `ColoredField` with lifetime from `GameDefinition`.
   */
  explicit ColoredField(common::Position& position);

  /**
   * \brief Construct new `ColoredField` from `Position`and lifetime.
   */
  ColoredField(game::common::Position& position, uint8_t lifetime);

  /**
   * \brief Return the `Position`of a `ColoredField`.
   */
  [[nodiscard]] common::Position GetPosition() const;

  /**
   * \brief Return the lifetime of a `ColoredField`.
   */
  [[nodiscard]] uint8_t GetLifetime() const;

  /**
   * \brief Reduce the lifetime of a `ColoredField`.
   */
  void ReduceLifetime();

  /**
   * \brief Serialize the object for transmission over the network.
   */
  void Serialize(std::vector<uint8_t>& output_vector) const override;

  static ColoredField Deserialize(std::vector<uint8_t>::iterator& input_iterator);

 private:
  common::Position position;
  uint8_t lifetime;
};

}  // namespace game::world
#endif  // CROSSING_CONSOLES_COLOREDFIELD_H
