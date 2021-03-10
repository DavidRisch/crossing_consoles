#ifndef CROSSING_CONSOLES_CONNECT_EVENT_H
#define CROSSING_CONSOLES_CONNECT_EVENT_H

#include "Event.h"

namespace communication::connection_layer {

class ConnectEvent : public Event {
 public:
  explicit ConnectEvent(ProtocolDefinition::address_t partner_id);

  [[nodiscard]] EventType GetType() const override;
};

}  // namespace communication::connection_layer

#endif  // CROSSING_CONSOLES_CONNECT_EVENT_H
