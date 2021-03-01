#ifndef CROSSING_CONSOLES_MOCK_INPUT_STREAM_H
#define CROSSING_CONSOLES_MOCK_INPUT_STREAM_H

#include <cstdint>
#include <list>
#include <vector>

#include "IInputStream.h"
#include "SocketHolder.h"

/**
 * \brief Implementation  of `IInputStream` used for testing.
 */
class MockInputStream : public IInputStream {
 public:
  size_t Read(uint8_t* receive_buffer, size_t max_length) override;

  void AddData(const std::vector<uint8_t>& new_data);

  bool IsEmpty() const;

 private:
  std::list<uint8_t> data;
};

#endif  // CROSSING_CONSOLES_MOCK_INPUT_STREAM_H
