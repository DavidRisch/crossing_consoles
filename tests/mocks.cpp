#include <gtest/gtest.h>

#include <iostream>
#include <thread>

#include "../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "../src/communication/byte_layer/byte_stream/SocketByteServer.h"

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
