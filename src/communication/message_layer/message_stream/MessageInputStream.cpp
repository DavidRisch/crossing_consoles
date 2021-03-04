#include "MessageInputStream.h"

#include <iostream>

#include "../../ProtocolDefinition.h"
#include "../../byte_layer/byte_stream/SocketByteStream.h"
#include "MessageCoder.h"

using namespace communication;
using namespace communication::message_layer;

MessageInputStream::MessageInputStream(byte_layer::IInputByteStream &input_stream)
    : input_stream(input_stream) {
}

std::shared_ptr<Message> MessageInputStream::ReceiveMessage(bool blocking) {
  while (true) {
    /*
    if(!blocking){
      if(!input_stream.HasInput()){
        return std::shared_ptr<Message>();
      }
    }
    */

    uint8_t current_byte = 0;
    // TODO: use ReadWithoutBlocking
    auto read_count = input_stream.ReadWithoutBlocking(&current_byte, 1);
    if (!blocking && read_count == 0) {
      return std::shared_ptr<Message>();
    }

    if (current_byte == ProtocolDefinition::flag) {
      return MessageCoder::Decode(input_stream, false);
    }
  }
}
