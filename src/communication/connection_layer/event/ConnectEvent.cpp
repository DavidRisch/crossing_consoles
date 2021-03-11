#include "ConnectEvent.h"

using namespace communication;
using namespace communication::connection_layer;

ConnectEvent::ConnectEvent(ProtocolDefinition::partner_id_t partner_id)
    : Event(partner_id) {
}

EventType ConnectEvent::GetType() const {
  return EventType::CONNECT;
}
