#ifndef CROSSING_CONSOLES_SERIALIZATION_UTILS_H
#define CROSSING_CONSOLES_SERIALIZATION_UTILS_H

#include <cstdint>
#include <string>
#include <vector>

namespace game::networking {

/**
 * \brief Provides methods for serialization and deserialization of objects.
 */
class SerializationUtils {
 public:
  /**
   * \brief Push all bytes of `object` at the end of `output_vector`.
   */
  template <class T>
  static void SerializeObject(const T &object, std::vector<uint8_t> &output_vector) {
    for (unsigned int i = 0; i < sizeof(T); ++i) {
      const uint8_t &byte = *(reinterpret_cast<const uint8_t *>(&object) + i);

      output_vector.push_back(byte);
    }
  }

  /**
   * \brief Returns a const reference to the object inside of the input vector and advances `input_iterator` by the
   * correct amount.
   */
  template <class T>
  static const T &DeserializeObject(std::vector<uint8_t>::iterator &input_iterator) {
    T &object = *(reinterpret_cast<T *>(&(*input_iterator)));
    for (unsigned int i = 0; i < sizeof(T); ++i) {
      input_iterator++;
    }
    return object;
  }

  /**
   * \brief Push the given string and its length at the end of `output_vector`.
   * \details Limited to a maximum length of 255 chars.
   */
  static void SerializeString(const std::string &string, std::vector<uint8_t> &output_vector);

  /**
   * \brief Decodes a string encoded by `SerializeString()`.
   */
  static std::string DeserializeString(std::vector<uint8_t>::iterator &input_iterator);
};
}  // namespace game::networking

#endif  // CROSSING_CONSOLES_SERIALIZATION_UTILS_H
