#ifndef CROSSING_CONSOLES_MESSAGE_H
#define CROSSING_CONSOLES_MESSAGE_H

#include <cstdint>
#include <string>
#include <vector>

typedef std::string ip_address_t;

class Message {
 public:
  Message(ip_address_t&, ip_address_t&, double);

  ip_address_t GetSource();
  ip_address_t GetDestination();
  double GetTimestamp() const;

 private:
  ip_address_t source;
  ip_address_t destination;
  double timestamp;
};

#endif  // CROSSING_CONSOLES_MESSAGE_H