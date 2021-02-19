#ifndef CROSSING_CONSOLES_SERVERCOMMUNICATION_H
#define CROSSING_CONSOLES_SERVERCOMMUNICATION_H

#include <list>

#include "ClientCommunication.h"
#include "ICommunication.h"
#include "ServerClientConnection.h"

class ServerCommunication : public ICommunication {
 public:
  ServerCommunication();
  void Send(const Message &m) override;
  Message Receive() override;
  void Broadcast();
  std::list<ServerClientConnection> GetClients();

 private:
};

#endif  // CROSSING_CONSOLES_SERVERCOMMUNICATION_H