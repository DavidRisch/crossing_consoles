#ifndef CROSSING_CONSOLES_SERVERCOMMUNICATION_H
#define CROSSING_CONSOLES_SERVERCOMMUNICATION_H

#include <list>

#include "ClientCommunication.h"
#include "ClientConnection.h"
#include "ICommunication.h"

class ServerCommunication : public ICommunication {
 public:
  ServerCommunication();
  void Send(const Message &m) override;
  Message Receive() override;
  void Broadcast();
  std::list<ClientConnection> GetClients();

 private:
};

#endif  // CROSSING_CONSOLES_SERVERCOMMUNICATION_H
