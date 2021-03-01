#ifndef CROSSING_CONSOLES_MESSAGE_OUTPUT_STREAM_H
#define CROSSING_CONSOLES_MESSAGE_OUTPUT_STREAM_H

#include <optional>
#include <vector>

#include "../../byte_layer/byte_stream/SocketByteStream.h"
#include "../message/Message.h"

/**
 * \brief Can send `Message`s to the given `IOutputByteStream`.
 */
class MessageOutputStream {
 public:
  explicit MessageOutputStream(IOutputByteStream &output_stream);

  void SendMessage(Message *message);

 private:
  IOutputByteStream &output_stream;
};

#endif  // CROSSING_CONSOLES_MESSAGE_OUTPUT_STREAM_H
