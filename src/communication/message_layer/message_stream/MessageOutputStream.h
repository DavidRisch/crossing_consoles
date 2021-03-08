#ifndef CROSSING_CONSOLES_MESSAGE_OUTPUT_STREAM_H
#define CROSSING_CONSOLES_MESSAGE_OUTPUT_STREAM_H

#include <optional>
#include <vector>

#include "../../byte_layer/byte_stream/SocketByteStream.h"
#include "../message/Message.h"

namespace communication {
namespace message_layer {

/**
 * \brief Can send `Message`s to the given `IOutputByteStream`.
 */
class MessageOutputStream {
 public:
  explicit MessageOutputStream(std::shared_ptr<byte_layer::IOutputByteStream> output_stream);

  void SendMessage(Message *message);

 private:
  std::shared_ptr<byte_layer::IOutputByteStream> output_stream;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_MESSAGE_OUTPUT_STREAM_H
