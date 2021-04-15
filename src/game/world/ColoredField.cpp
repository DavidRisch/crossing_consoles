#include "ColoredField.h"

#include "../GameDefinition.h"

using namespace game;
using namespace world;

ColoredField::ColoredField(game::common::Position& position)
    : position(position) {
  lifetime = GameDefinition::colored_field_lifetime;
}

ColoredField::ColoredField(game::common::Position& position, uint8_t lifetime)
    : position(position)
    , lifetime(lifetime) {
}

common::Position game::world::ColoredField::GetPosition() const {
  return position;
}

uint8_t ColoredField::GetLifetime() const {
  return lifetime;
}

void ColoredField::ReduceLifetime() {
  lifetime--;
}

void ColoredField::Serialize(std::vector<uint8_t>& output_vector) const {
  position.Serialize(output_vector);
  output_vector.push_back(lifetime);
}

ColoredField ColoredField::Deserialize(std::vector<uint8_t>::iterator& input_iterator) {
  common::Position deserialized_position = common::Position::Deserialize(input_iterator);
  uint8_t deserialized_lifetime = *input_iterator++;

  ColoredField colored_field(deserialized_position, deserialized_lifetime);
  return colored_field;
}
