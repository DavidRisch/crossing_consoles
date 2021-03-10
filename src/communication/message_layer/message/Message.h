#ifndef CROSSING_CONSOLES_MESSAGE_H
#define CROSSING_CONSOLES_MESSAGE_H

#include <cstdint>
#include <string>
#include <vector>

#include "../../ProtocolDefinition.h"
#include "MessageMetaData.h"

namespace communication {
namespace message_layer {

enum class MessageType : char {
  CONNECTION_REQUEST = 0,
  CONNECTION_RESPONSE,
  PAYLOAD,
  ACKNOWLEDGE,
  KEEP_ALIVE,
  HIGHEST_ELEMENT = KEEP_ALIVE
};

class Message {
 public:
  // Draft message
  Message(ProtocolDefinition::address_t address);

  // Received message
  Message(ProtocolDefinition::address_t address, ProtocolDefinition::sequence_t sequence);

  ProtocolDefinition::sequence_t GetMessageSequence() const;
  void SetMessageSequence(ProtocolDefinition::sequence_t new_sequence);

  [[nodiscard]] ProtocolDefinition::address_t GetAddress() const;
  [[nodiscard]] virtual MessageType GetMessageType() const;
  [[nodiscard]] MessageMetaData GetMessageMetaData() const;

  void SetTimestampReceived(std::chrono::steady_clock::time_point timestamp);
  void SetTimestampSent(std::chrono::steady_clock::time_point timestamp);

 private:
  ProtocolDefinition::address_t address;
  MessageMetaData meta_data;
  ProtocolDefinition::sequence_t sequence;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_MESSAGE_H
