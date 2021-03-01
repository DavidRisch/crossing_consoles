#ifndef CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
#define CROSSING_CONSOLES_PROTOCOL_DEFINITION_H

/**
 * \brief Defines constants used in the protocol.
 */
class ProtocolDefinition {
 public:
  /// Arbitrary constant, used to mark the start of a message.
  constexpr static const uint8_t start_sequence[] = {0x12u, 0x34u, 0x56u, 0x78u};

  /// A type large enough to hold `start_sequence`.
  typedef uint32_t start_sequence_t;
  static_assert(sizeof(start_sequence_t) >= sizeof(start_sequence));

  [[nodiscard]] static start_sequence_t GetNumericStartSequence() {
    uint32_t numeric_start_sequence = 0;
    for (unsigned int i = 0; i < sizeof(start_sequence); ++i) {
      numeric_start_sequence |= start_sequence[i] << (8 * (sizeof(start_sequence) - 1 - i));
    }
    return numeric_start_sequence;
  }

  typedef uint16_t payload_length_t;
};

#endif  // CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
