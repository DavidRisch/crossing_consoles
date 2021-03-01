#ifndef CROSSING_CONSOLES_MOCK_BIDIRECTIONAL_BYTE_STREAM_H
#define CROSSING_CONSOLES_MOCK_BIDIRECTIONAL_BYTE_STREAM_H

#include <cstdint>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

#include "IInputByteStream.h"
#include "IOutputByteStream.h"
#include "SocketHolder.h"

namespace communication {
namespace byte_layer {

/**
 * \brief Implementation of `IInputByteStream` and `IOutputByteStream` used for testing.
 */
class MockBidirectionalByteStream : public IInputByteStream, public IOutputByteStream {
 public:
  /**
   * \brief Create two linked instances of `MockBidirectionalByteStream`, data sent with one will be received by the
   * other.
   */
  static std::pair<std::shared_ptr<MockBidirectionalByteStream>, std::shared_ptr<MockBidirectionalByteStream>>
  CreatePair();

  size_t Read(uint8_t* receive_buffer, size_t max_length) override;

  void Send(const uint8_t* send_buffer, size_t length) override;

 private:
  MockBidirectionalByteStream(std::shared_ptr<std::mutex> mutex);

  std::list<uint8_t> input_data;

  std::weak_ptr<MockBidirectionalByteStream> other_end_weak;

  std::shared_ptr<std::mutex> mutex;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_MOCK_BIDIRECTIONAL_BYTE_STREAM_H
