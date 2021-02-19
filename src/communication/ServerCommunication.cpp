#include "ServerCommunication.h"

void ServerCommunication::Send(const Message& m) {
}

Message ServerCommunication::Receive() {
  return Message();
}

void ServerCommunication::Broadcast() {
}

std::list<ClientConnection> ServerCommunication::GetClients() {
  return std::list<ClientConnection>();
}

ServerCommunication::ServerCommunication() {
}
