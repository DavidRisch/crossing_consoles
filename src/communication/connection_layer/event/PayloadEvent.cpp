#include "PayloadEvent.h"

#include <utility>

using namespace communication;
using namespace communication::connection_layer;

PayloadEvent::PayloadEvent(ProtocolDefinition::partner_id_t clientId, std::vector<uint8_t> payload)
    : Event(clientId)
    , payload(std::move(payload)) {
}

EventType PayloadEvent::GetType() const {
  return EventType::PAYLOAD;
}

const std::vector<uint8_t>& PayloadEvent::GetPayload() const {
  return payload;
}
