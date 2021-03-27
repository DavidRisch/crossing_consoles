#include <gtest/gtest.h>

#include <iostream>
#include <thread>

#include "../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../src/communication/byte_layer/byte_stream/MockInputStream.h"
#include "../src/communication/byte_layer/byte_stream/SocketByteServer.h"

using namespace communication;
using namespace communication::byte_layer;

TEST(Mocks, MockBidirectionalByteStream) {
  const uint8_t first_to_second[] = {'t', 'e', 's', 't'};
  const uint8_t second_to_first[] = {'a', 'b', 'c', 'd'};

  uint8_t first_received[sizeof(first_to_second)];
  uint8_t second_received[sizeof(second_to_first)];

  auto pair = MockBidirectionalByteStream::CreatePair();

  // first to second
  {
    pair.first->Send(second_to_first, sizeof(second_to_first));
    int read_count = pair.second->Read(second_received, sizeof(second_received));
    EXPECT_EQ(read_count, sizeof(second_received));
    for (size_t i = 0; i < sizeof(second_to_first); ++i) {
      EXPECT_EQ(second_to_first[i], second_received[i]);
    }
  }

  // second to first
  {
    pair.second->Send(first_to_second, sizeof(first_to_second));
    int read_count = pair.first->Read(first_received, sizeof(first_received));
    EXPECT_EQ(read_count, sizeof(first_received));
    for (size_t i = 0; i < sizeof(first_to_second); ++i) {
      EXPECT_EQ(first_to_second[i], first_received[i]);
    }
  }
}

TEST(Mocks, MockInputStream) {
  // write some data to MockInputStream
  const std::vector<uint8_t> original_data = {1, 2, 3, 4};
  MockInputStream mock_input_stream;
  mock_input_stream.AddData(original_data);

  // Stream has to be non-empty
  ASSERT_FALSE(mock_input_stream.IsEmpty());

  // check if read data from Stream equals the original data
  uint8_t received_data;
  int i = 0;
  while (!mock_input_stream.IsEmpty()) {
    mock_input_stream.Read(&received_data, 1);
    ASSERT_EQ(original_data[i], received_data);
    i++;
  }

  // check that stream is now empty
  ASSERT_TRUE(mock_input_stream.IsEmpty());
}
