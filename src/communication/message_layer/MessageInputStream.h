#ifndef CROSSING_CONSOLES_MESSAGE_INPUT_STREAM_H
#define CROSSING_CONSOLES_MESSAGE_INPUT_STREAM_H

#include <optional>
#include <vector>

#include "../low_level/ByteStream.h"
#include "../messages/Message.h"

/**
 * \brief Can receive `Message`s from the given `IInputStream`.
 */
class MessageInputStream {
 public:
  explicit MessageInputStream(IInputStream &input_stream);

  std::unique_ptr<Message> ReceiveMessage();

 private:
  IInputStream &input_stream;
};

#endif  // CROSSING_CONSOLES_MESSAGE_INPUT_STREAM_H
