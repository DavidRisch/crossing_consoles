#ifndef CROSSING_CONSOLES_MESSAGE_INPUT_STREAM_H
#define CROSSING_CONSOLES_MESSAGE_INPUT_STREAM_H

#include <optional>
#include <vector>

#include "../../byte_layer/byte_stream/SocketByteStream.h"
#include "../message/Message.h"

namespace communication {
namespace message_layer {

/**
 * \brief Can receive `Message`s from the given `IInputByteStream`.
 */
class MessageInputStream {
 public:
  explicit MessageInputStream(std::shared_ptr<byte_layer::IInputByteStream> input_stream);

  std::shared_ptr<Message> ReceiveMessage(ProtocolDefinition::address_t address, bool blocking = true);

 private:
  std::shared_ptr<byte_layer::IInputByteStream> input_stream;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_MESSAGE_INPUT_STREAM_H
