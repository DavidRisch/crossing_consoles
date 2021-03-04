#include "MockBidirectionalByteStream.h"

#include <utility>

using namespace communication;
using namespace communication::byte_layer;

std::pair<std::shared_ptr<MockBidirectionalByteStream>, std::shared_ptr<MockBidirectionalByteStream>>
MockBidirectionalByteStream::CreatePair() {
  auto mutex = std::make_shared<std::mutex>();

  std::shared_ptr<MockBidirectionalByteStream> first(new MockBidirectionalByteStream(mutex));
  std::shared_ptr<MockBidirectionalByteStream> second(new MockBidirectionalByteStream(mutex));

  first->other_end_weak = second;
  second->other_end_weak = first;

  return std::pair<std::shared_ptr<MockBidirectionalByteStream>, std::shared_ptr<MockBidirectionalByteStream>>(first,
                                                                                                               second);
}

MockBidirectionalByteStream::MockBidirectionalByteStream(std::shared_ptr<std::mutex> mutex)
    : mutex(std::move(mutex)) {
}

size_t MockBidirectionalByteStream::Read(uint8_t *receive_buffer, size_t max_length) {
  std::lock_guard<std::mutex> lock_guard(*mutex);

  for (unsigned int i = 0; i < max_length; ++i) {
    if (!input_data.empty()) {
      receive_buffer[i] = *input_data.begin();
      input_data.pop_front();
    } else {
      return static_cast<size_t>(i);
    }
  }
  return max_length;
}

void MockBidirectionalByteStream::Send(const uint8_t *send_buffer, size_t length) {
  std::lock_guard<std::mutex> lock_guard(*mutex);

  auto other_end = other_end_weak.lock();

  for (size_t i = 0; i < length; ++i) {
    other_end->input_data.push_back(send_buffer[i]);
  }
}
