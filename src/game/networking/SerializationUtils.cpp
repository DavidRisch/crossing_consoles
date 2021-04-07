#include "SerializationUtils.h"

#include <cassert>

using namespace game::networking;

void SerializationUtils::SerializeString(const std::string &input, std::vector<uint8_t> &output_vector) {
  assert(input.size() <= 255);
  output_vector.push_back(input.size());
  for (const auto &character : input) {
    output_vector.push_back(character);
  }
}

std::string SerializationUtils::DeserializeString(std::vector<uint8_t>::iterator &input_iterator) {
  std::string output;
  auto length = *input_iterator++;
  for (int i = 0; i < length; ++i) {
    output += (char)*input_iterator++;
  }
  return output;
}
