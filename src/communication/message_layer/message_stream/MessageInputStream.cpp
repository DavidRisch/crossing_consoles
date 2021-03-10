#include "MessageInputStream.h"

#include <utility>

#include "MessageCoder.h"

using namespace communication;
using namespace communication::message_layer;

MessageInputStream::MessageInputStream(std::shared_ptr<byte_layer::IInputByteStream> input_stream)
    : input_stream(std::move(input_stream)) {
}

std::shared_ptr<Message> MessageInputStream::ReceiveMessage(bool blocking) {
  while (true) {
    uint8_t current_byte = 0;
    auto read_count = input_stream->ReadWithoutBlocking(&current_byte, 1);

    if (!blocking && read_count == 0) {
      return std::shared_ptr<Message>();
    }

    if (current_byte == ProtocolDefinition::flag) {
      return MessageCoder::Decode(*input_stream, false);
    }
  }
}
