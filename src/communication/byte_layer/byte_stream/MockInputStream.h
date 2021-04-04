#ifndef CROSSING_CONSOLES_MOCK_INPUT_STREAM_H
#define CROSSING_CONSOLES_MOCK_INPUT_STREAM_H

#include <cstdint>
#include <list>
#include <stdexcept>
#include <vector>

#include "IInputByteStream.h"
#include "SocketHolder.h"

namespace communication {
namespace byte_layer {

/**
 * \brief Implementation  of `IInputByteStream` used for testing.
 */
class MockInputStream : public IInputByteStream {
 public:
  size_t Read(uint8_t* receive_buffer, size_t max_length) override;

  /**
   * \brief Add data to the Input Stream.
   */
  void AddData(const std::vector<uint8_t>& new_data);

  /**
   * \brief Check if the Stream is empty.
   */
  bool IsEmpty() const;

  /**
   * \brief Check if there is data on the Stream.
   * @return
   */
  bool HasInput() override {
    return !data.empty();
  }

 private:
  std::list<uint8_t> data;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_MOCK_INPUT_STREAM_H
