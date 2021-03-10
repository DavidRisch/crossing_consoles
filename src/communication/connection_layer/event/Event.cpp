#include "Event.h"

using namespace communication;
using namespace communication::connection_layer;

Event::Event(ProtocolDefinition::address_t partner_id)
    : partner_id(partner_id) {
}

ProtocolDefinition::address_t Event::GetPartnerId() const {
  return partner_id;
}
