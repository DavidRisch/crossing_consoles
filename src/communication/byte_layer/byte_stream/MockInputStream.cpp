#include "MockInputStream.h"

using namespace communication;
using namespace communication::byte_layer;

size_t MockInputStream::Read(uint8_t* receive_buffer, size_t max_length) {
  for (unsigned int i = 0; i < max_length; ++i) {
    if (!data.empty()) {
      receive_buffer[i] = *data.begin();
      data.pop_front();
    } else {
      return static_cast<size_t>(i);
    }
  }
  return max_length;
}

void MockInputStream::AddData(const std::vector<uint8_t>& new_data) {
  for (const auto& new_byte : new_data) {
    data.push_back(new_byte);
  }
}
bool MockInputStream::IsEmpty() const {
  return data.empty();
}
