#ifndef CROSSING_CONSOLES_MESSAGE_INPUT_STREAM_H
#define CROSSING_CONSOLES_MESSAGE_INPUT_STREAM_H

#include <optional>
#include <vector>

#include "../../byte_layer/byte_stream/SocketByteStream.h"
#include "../message/Message.h"

/**
 * \brief Can receive `Message`s from the given `IInputByteStream`.
 */
class MessageInputStream {
 public:
  explicit MessageInputStream(IInputByteStream &input_stream);

  std::shared_ptr<Message> ReceiveMessage();

 private:
  IInputByteStream &input_stream;
};

#endif  // CROSSING_CONSOLES_MESSAGE_INPUT_STREAM_H
