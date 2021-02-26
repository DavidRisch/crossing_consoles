#ifndef CROSSING_CONSOLES_MESSAGE_CODER_H
#define CROSSING_CONSOLES_MESSAGE_CODER_H

#include <memory>

#include "Message.h"

/**
 * \brief Contains static methods for `Message` en- and decoding.
 */
class MessageCoder {
 public:
  // TODO: CRC implementation
  typedef uint32_t crc_t;
  static const int crc_length = 4;

  /**
   * \brief Encode message of any type.
   */
  static std::vector<uint8_t> Encode(Message* message);

  /**
   * \brief Decode message of any type.
   * \details If the encoded Message is larger than `receive_buffer_length`, an `InputTooShortException` is thrown.
   */
  static std::unique_ptr<Message> Decode(const uint8_t* receive_buffer, size_t receive_buffer_length);

  class InputTooShortException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "Message decoding failed, input was shorter than expected.";
    }
  };

  class CrcIncorrectException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "Message decoding failed, read CRC did not match expected CRC.";
    }
  };
};

#endif  // CROSSING_CONSOLES_MESSAGE_CODER_H
