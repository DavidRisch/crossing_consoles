#include "MessageInputStream.h"

#include <iostream>

#include "../../ProtocolDefinition.h"
#include "MessageCoder.h"

using namespace communication;
using namespace communication::message_layer;

MessageInputStream::MessageInputStream(byte_layer::IInputByteStream &input_stream)
    : input_stream(input_stream) {
}

std::shared_ptr<Message> MessageInputStream::ReceiveMessage() {
  while (true) {
    uint8_t current_byte = 0;
    input_stream.Read(&current_byte, 1);

    if (current_byte == ProtocolDefinition::flag) {
      return MessageCoder::Decode(input_stream, false);
    }
  }
}
