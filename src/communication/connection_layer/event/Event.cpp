#include "Event.h"

using namespace communication;
using namespace communication::connection_layer;

Event::Event(ProtocolDefinition::partner_id_t partner_id)
    : partner_id(partner_id) {
}

ProtocolDefinition::partner_id_t Event::GetPartnerId() const {
  return partner_id;
}
