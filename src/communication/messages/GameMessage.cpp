#include "GameMessage.h"

#include <utility>

GameMessage::GameMessage(ip_address_t& source, ip_address_t& destination, double timestamp, world_t& world)
    : Message(source, destination, timestamp) {
  this->world = world;
}
world_t GameMessage::GetWorld() {
  return this->world;
}
