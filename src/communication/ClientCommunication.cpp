#include "ClientCommunication.h"

ClientCommunication::ClientCommunication() {
}

void ClientCommunication::Send(const Message& m) {
}

Message ClientCommunication::Receive() {
  return Message(0, KEEP_ALIVE);   // TODO
}
