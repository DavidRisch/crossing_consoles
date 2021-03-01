#ifndef CROSSING_CONSOLES_CRCHANDLER_H
#define CROSSING_CONSOLES_CRCHANDLER_H

#include <boost/crc.hpp>

namespace communication {
namespace message_layer {

typedef uint32_t crc_value_t;  // depends on CRC algorithm

class CRCHandler {
  /**
   * \brief Create and check CRC32 checksums.
   * \details The used CRC32 algorithm is described in
   * https://en.wikipedia.org/wiki/Cyclic_redundancy_check#CRC-32_algorithm
   */
 public:
  CRCHandler();
  /**
   * \brief Calculate and return the associated CRC32 value of given data
   */
  static crc_value_t CalculateCRCValue(const uint8_t* data, size_t data_length);
  /**
   * \brief Compare the associated CRC32 value of given data with the provided checksum
   */
  static bool CheckCRCValue(const uint8_t* data, size_t data_length, const crc_value_t checksum);
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CRCHANDLER_H
