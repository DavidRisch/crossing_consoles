#ifndef CROSSING_CONSOLES_CLIENTCONNECTION_H
#define CROSSING_CONSOLES_CLIENTCONNECTION_H

typedef int client_id_t;

class ClientConnection {
 public:
  client_id_t GetId();

 private:
  client_id_t id;
  // Socket
};

#endif  // CROSSING_CONSOLES_CLIENTCONNECTION_H
