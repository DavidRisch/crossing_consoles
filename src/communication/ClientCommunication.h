#ifndef CROSSING_CONSOLES_CLIENTCOMMUNICATION_H
#define CROSSING_CONSOLES_CLIENTCOMMUNICATION_H

#include "ICommunication.h"

class ClientCommunication : public ICommunication {
 public:
  explicit ClientCommunication();
  void Send(const Message &m) override;
  Message Receive() override;
};

#endif  // CROSSING_CONSOLES_CLIENTCOMMUNICATION_H
