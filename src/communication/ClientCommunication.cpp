#include "ClientCommunication.h"

#include "messages/Message.h"

ClientCommunication::ClientCommunication() {
}

void ClientCommunication::Send(const Message& m) {
}

Message ClientCommunication::Receive() {
  return Message(0);  // TODO
}
