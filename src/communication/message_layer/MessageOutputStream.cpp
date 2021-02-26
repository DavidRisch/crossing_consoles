#include "MessageOutputStream.h"

#include <iostream>

#include "../ProtocolDefinition.h"
#include "../messages/MessageCoder.h"
#include "MessageInputStream.h"

using namespace std;

MessageOutputStream::MessageOutputStream(IOutputStream &output_stream)
    : output_stream(output_stream) {
}

void MessageOutputStream::SendMessage(Message *message) {
  auto encoded_message = MessageCoder::Encode(message);
  output_stream.Send(encoded_message.data(), encoded_message.size());
}
