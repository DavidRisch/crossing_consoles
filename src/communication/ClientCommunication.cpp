#include "ClientCommunication.h"

ClientCommunication::ClientCommunication() {
}

void ClientCommunication::Send(const Message& m) {
}

Message ClientCommunication::Receive() {
  return Message((ip_address_t&)"", (ip_address_t&)"", 0.);  // TODO
}
