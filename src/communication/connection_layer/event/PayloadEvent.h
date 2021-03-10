#ifndef CROSSING_CONSOLES_PAYLOAD_EVENT_H
#define CROSSING_CONSOLES_PAYLOAD_EVENT_H

#include <cstdint>
#include <vector>

#include "Event.h"

namespace communication::connection_layer {

class PayloadEvent : public Event {
 public:
  explicit PayloadEvent(ProtocolDefinition::address_t client_id, std::vector<uint8_t> payload);

  [[nodiscard]] EventType GetType() const override;

  [[nodiscard]] const std::vector<uint8_t>& GetPayload() const;

 private:
  std::vector<uint8_t> payload;
};

}  // namespace communication::connection_layer

#endif  // CROSSING_CONSOLES_PAYLOAD_EVENT_H
