#ifndef CROSSING_CONSOLES_CRCHANDLER_H
#define CROSSING_CONSOLES_CRCHANDLER_H

#include <boost/crc.hpp>

namespace communication {
namespace message_layer {

typedef uint32_t crc_value_t;  // depends on CRC algorithm

/**
 * \brief Create and check CRC32 checksums.
 * \details The used CRC32 algorithm is described in
 * https://en.wikipedia.org/wiki/Cyclic_redundancy_check#CRC-32_algorithm
 */
class CRCHandler {
 public:
  CRCHandler();
  /**
   * \brief Calculate and return the associated CRC32 value of given data
   */
  static crc_value_t CalculateCRCValue(const uint8_t* data, size_t data_length);

  /**
   * \brief Compare the associated CRC32 value of given data with the provided checksum
   */
  static bool CalculateAndCheckCRCValue(const uint8_t* data, size_t data_length, const crc_value_t checksum);

  /**
   * \brief Compare the associated CRC32 value of `crc_handler` with the provided checksum
   * \details All data of `crc_handler` will be reset.
   */
  bool CheckCRCValue(const crc_value_t checksum);

  /**
   * \brief Add data to `crc_data`
   */
  void AppendData(const uint8_t* data, const size_t data_length);

  /**
   * \brief Add byte to `crc_data`
   */
  void AppendByte(const uint8_t byte);

 private:
  boost::crc_32_type crc_data;
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_CRCHANDLER_H
