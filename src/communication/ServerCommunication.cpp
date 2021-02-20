#include "ServerCommunication.h"

void ServerCommunication::Send(const Message& m) {
}

Message ServerCommunication::Receive() {
  return Message((ip_address_t&)"", (ip_address_t&)"", 0.);  // TODO
}

void ServerCommunication::Broadcast() {
}

std::list<ServerClientConnection> ServerCommunication::GetClients() {
  return std::list<ServerClientConnection>();
}

ServerCommunication::ServerCommunication() {
}
