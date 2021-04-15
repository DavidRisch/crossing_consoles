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
  NOT_ACKNOWLEDGE,
  KEEP_ALIVE,
  CONNECTION_RESET,
  HIGHEST_ELEMENT = CONNECTION_RESET
};

const char* message_type_to_string(MessageType value);

/**
 * \brief Base class for messages of different types.
 */
class Message {
 public:
  // Draft message
  Message();

  // Received message
  Message(ProtocolDefinition::sequence_t sequence);

  virtual ~Message() = default;

  ProtocolDefinition::sequence_t GetMessageSequence() const;
  void SetMessageSequence(ProtocolDefinition::sequence_t new_sequence);

  [[nodiscard]] virtual MessageType GetMessageType() const = 0;
  [[nodiscard]] MessageMetaData GetMessageMetaData() const;

  void SetTimestampReceived(std::chrono::steady_clock::time_point timestamp);
  void SetTimestampSent(std::chrono::steady_clock::time_point timestamp);

 private:
  MessageMetaData meta_data;
  ProtocolDefinition::sequence_t sequence;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_MESSAGE_H
