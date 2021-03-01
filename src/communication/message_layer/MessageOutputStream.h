#ifndef CROSSING_CONSOLES_MESSAGE_OUTPUT_STREAM_H
#define CROSSING_CONSOLES_MESSAGE_OUTPUT_STREAM_H

#include <optional>
#include <vector>

#include "../low_level/ByteStream.h"
#include "../messages/Message.h"

/**
 * \brief Can send `Message`s to the given `IOutputStream`.
 */
class MessageOutputStream {
 public:
  explicit MessageOutputStream(IOutputStream &output_stream);

  void SendMessage(Message *message);

 private:
  IOutputStream &output_stream;
};

#endif  // CROSSING_CONSOLES_MESSAGE_OUTPUT_STREAM_H
