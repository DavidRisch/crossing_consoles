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
  [[nodiscard]] ProtocolDefinition::partner_id_t GetPartnerId() const;

  [[nodiscard]] virtual EventType GetType() const = 0;

  explicit Event(ProtocolDefinition::partner_id_t partner_id);

  virtual ~Event() = default;

 private:
  /**
   * Id of the partner to which this event relates.
   */
  ProtocolDefinition::partner_id_t partner_id;
};

}  // namespace communication::connection_layer

#endif  // CROSSING_CONSOLES_EVENT_H
