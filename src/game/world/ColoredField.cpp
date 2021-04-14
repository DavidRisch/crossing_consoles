#include "ColoredField.h"

#include <iostream>

using namespace game;
using namespace world;

ColoredField::ColoredField(game::common::Position& position)
    : position(position) {
  lifetime = 5;  // world cycles in the Game Server
}

common::Position game::world::ColoredField::GetPosition() const {
  return position;
}

uint8_t ColoredField::GetLifetime() const {
  return lifetime;
}

void ColoredField::SetLifetime(uint8_t new_lifetime) {
  lifetime = new_lifetime;
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

  ColoredField colored_field(deserialized_position);
  colored_field.SetLifetime(deserialized_lifetime);

  return colored_field;
}
