#include "ServerCommunication.h"

void ServerCommunication::Send(const Message& m) {
}

Message ServerCommunication::Receive() {
  return Message();
}

void ServerCommunication::Broadcast() {
}

std::list<ServerClientConnection> ServerCommunication::GetClients() {
  return std::list<ServerClientConnection>();
}

ServerCommunication::ServerCommunication() {
}
