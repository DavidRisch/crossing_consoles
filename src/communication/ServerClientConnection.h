#ifndef CROSSING_CONSOLES_SERVERCLIENTCONNECTION_H
#define CROSSING_CONSOLES_SERVERCLIENTCONNECTION_H

typedef int client_id_t;

class ServerClientConnection {
 public:
  client_id_t GetId();

 private:
  client_id_t id;
  // Socket
};

#endif  // CROSSING_CONSOLES_SERVERCLIENTCONNECTION_H
