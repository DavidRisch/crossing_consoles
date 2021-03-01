#include "MessageOutputStream.h"

#include "MessageCoder.h"

using namespace communication;
using namespace communication::message_layer;

MessageOutputStream::MessageOutputStream(byte_layer::IOutputByteStream &output_stream)
    : output_stream(output_stream) {
}

void MessageOutputStream::SendMessage(Message *message) {
  auto encoded_message = MessageCoder::Encode(message);
  output_stream.Send(encoded_message.data(), encoded_message.size());
}
