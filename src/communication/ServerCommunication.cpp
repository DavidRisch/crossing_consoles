#include "ServerCommunication.h"

void ServerCommunication::Send(const Message& m) {
}

Message ServerCommunication::Receive() {
  // TODO
  return Message(0);
}

void ServerCommunication::Broadcast() {
}

std::list<ServerClientConnection> ServerCommunication::GetClients() {
  return std::list<ServerClientConnection>();
}

ServerCommunication::ServerCommunication() {
}
