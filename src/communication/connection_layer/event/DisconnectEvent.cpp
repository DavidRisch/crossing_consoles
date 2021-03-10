#include "DisconnectEvent.h"

using namespace communication;
using namespace communication::connection_layer;

EventType DisconnectEvent::GetType() const {
  return EventType::DISCONNECT;
}

DisconnectEvent::DisconnectEvent(ProtocolDefinition::address_t partner_id)
    : Event(partner_id) {
}
