#ifndef CROSSING_CONSOLES_EVENT_H
#define CROSSING_CONSOLES_EVENT_H

#include "../../ProtocolDefinition.h"

namespace communication::connection_layer {

enum class EventType : char {
  CONNECT,
  DISCONNECT,
  PAYLOAD,
};

/**
 * \brief Represents some kind of change in one of the `Connection`s of a `ConnectionManager`.
 */
class Event {
 public:
  [[nodiscard]] ProtocolDefinition::address_t GetPartnerId() const;

  [[nodiscard]] virtual EventType GetType() const = 0;

  explicit Event(ProtocolDefinition::address_t partner_id);

 private:
  /**
   * Id of the partner to which this event relates.
   */
  ProtocolDefinition::address_t partner_id;
};

}  // namespace communication::connection_layer

#endif  // CROSSING_CONSOLES_EVENT_H
