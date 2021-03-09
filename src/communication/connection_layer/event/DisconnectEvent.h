#ifndef CROSSING_CONSOLES_DISCONNECT_EVENT_H
#define CROSSING_CONSOLES_DISCONNECT_EVENT_H

#include "Event.h"

namespace communication::connection_layer {

class DisconnectEvent : public Event {
 public:
  explicit DisconnectEvent(ProtocolDefinition::partner_id_t partner_id);

  [[nodiscard]] EventType GetType() const override;
};

}  // namespace communication::connection_layer

#endif  // CROSSING_CONSOLES_DISCONNECT_EVENT_H
