#ifndef CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
#define CROSSING_CONSOLES_PROTOCOL_DEFINITION_H

#include <time.h>

#include <cstdint>

namespace communication {

/**
 * \brief Defines constants used in the protocol.
 */
class ProtocolDefinition {
 public:
  /// A type large enough to hold `flag`.
  typedef uint8_t flag_t;

  /// Arbitrary constant, used to mark the start of a message.
  static const flag_t flag = 0x86u;

  /// A type large enough to hold `escape`.
  typedef uint8_t escape_t;

  /// Arbitrary constant, used as the `escape` sequence.
  constexpr static const escape_t escape = 0x87u;

  typedef uint16_t payload_length_t;

  /// A type large enough to hold message `sequence`.
  typedef uint16_t sequence_t;

  /// Set timeout duration
  static const time_t timeout = static_cast<time_t>(3);
};

}  // namespace communication

#endif  // CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
