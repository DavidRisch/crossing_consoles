#ifndef CROSSING_CONSOLES_MESSAGE_CODER_H
#define CROSSING_CONSOLES_MESSAGE_CODER_H

#include <memory>

#include "../../byte_layer/byte_stream/IInputByteStream.h"
#include "../crc/CRCHandler.h"
#include "../message/Message.h"

namespace communication {
namespace message_layer {

/**
 * \brief Contains static methods for `Message` en- and decoding.
 */
class MessageCoder {
 public:
  static const int crc_length = sizeof(crc_value_t);

  /**
   * \brief Encode message of any type.
   */
  static std::vector<uint8_t> Encode(Message* message);

  /**
   * \brief Decode message of any type.
   */
  static std::shared_ptr<Message> Decode(byte_layer::IInputByteStream& stream, bool expect_start_sequence = true);

  /**
   * \brief Thrown if message en-/ or decoding fails for any reason.
   * \details `CrcIncorrectException' and `InvalidMessageException` distinguish reason of failure for debugging
   * purposes.
   */
  class DecodeFailedException : public std::exception {};

  /**
   * \brief Thrown if read CRC value does not match expected CRC value.
   */
  class CrcIncorrectException : public DecodeFailedException {
    [[nodiscard]] const char* what() const noexcept override {
      return "Message decoding failed, read CRC did not match expected CRC.";
    }
  };

  /**
   * \brief Thrown if flags or CRC value are incorrect.
   */
  class InvalidMessageException : public DecodeFailedException {
   private:
    std::string output_message;

   public:
    explicit InvalidMessageException(std::string reason)
        : output_message("Invalid message: " + reason){};

    [[nodiscard]] const char* what() const noexcept override {
      return output_message.c_str();
    }
  };
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_MESSAGE_CODER_H
