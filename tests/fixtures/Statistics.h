#ifndef CROSSING_CONSOLES_STATISTICS_H
#define CROSSING_CONSOLES_STATISTICS_H

#include <gtest/gtest.h>

#include "../../src/communication/byte_layer/byte_stream/MockBidirectionalByteStream.h"
#include "Connections.h"

class Statistics : public Connections {
 public:
  void establish_connections() {
    auto stream_pair = MockBidirectionalByteStream::CreatePair();
    auto &server_side = stream_pair.first;
    auto &client_side = stream_pair.second;

    auto client_message_input_stream = std::make_shared<MessageInputStream>(client_side);
    auto server_message_output_stream = std::make_shared<MessageOutputStream>(server_side);

    auto server_message_input_stream = std::make_shared<MessageInputStream>(server_side);
    auto client_message_output_stream = std::make_shared<MessageOutputStream>(client_side);

    make_connections(client_message_input_stream, client_message_output_stream, server_message_input_stream,
                     server_message_output_stream);
  };
};

#endif  // CROSSING_CONSOLES_STATISTICS_H
