#ifndef CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
#define CROSSING_CONSOLES_PROTOCOL_DEFINITION_H

#include <time.h>

#include <chrono>
#include <cstdint>
#include <string>

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

  ///  Arbitrary constant, used as the last byte of a message.
  constexpr static const uint8_t end_marker = 0x88u;

  typedef uint16_t payload_length_t;

  /// A type large enough to hold message `sequence`.
  typedef uint16_t sequence_t;

  typedef std::chrono::duration<int64_t, std::milli> timeout_t;

  /// Set timeout duration
  static constexpr const timeout_t timeout = std::chrono::milliseconds(1000);

  /// `timeout` divided by `resend_denominator` is the interval after which a resend is triggered.
  constexpr static const double resend_denominator = 10;
  static_assert(resend_denominator > 1.0);

  /// Set keep alive denominator
  /// `timeout` divided by `keep_alive_denominator` describes the interval after which a `KeepAliveMessage' is sent
  constexpr static const double keep_alive_denominator = 2;
  static_assert(keep_alive_denominator > 0);

  /// Used to reference connection partners.
  typedef unsigned int partner_id_t;
  /// partner id of the server. A constant because there is only a single server.
  static constexpr partner_id_t server_partner_id = 0;

#ifdef USE_UNIX_SOCKET
  static std::string GetUnixSocketPath(uint16_t port) {
    return "/tmp/crossing_consoles_socket_" + std::to_string(port);
  }
#endif
};

}  // namespace communication

#endif  // CROSSING_CONSOLES_PROTOCOL_DEFINITION_H
