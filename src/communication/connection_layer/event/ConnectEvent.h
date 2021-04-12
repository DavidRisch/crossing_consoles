#ifndef CROSSING_CONSOLES_CONNECT_EVENT_H
#define CROSSING_CONSOLES_CONNECT_EVENT_H

#include "Event.h"

namespace communication::connection_layer {

/**
 * \brief Shows that a `Connection` has been established.
 */
class ConnectEvent : public Event {
 public:
  explicit ConnectEvent(ProtocolDefinition::partner_id_t partner_id);

  [[nodiscard]] EventType GetType() const override;
};

}  // namespace communication::connection_layer

#endif  // CROSSING_CONSOLES_CONNECT_EVENT_H
