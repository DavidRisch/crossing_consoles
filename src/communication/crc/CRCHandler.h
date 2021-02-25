#ifndef CROSSING_CONSOLES_CRCHANDLER_H
#define CROSSING_CONSOLES_CRCHANDLER_H

#include <boost/crc.hpp>
typedef uint32_t crc_value_t;  // depends on CRC algorithm

class CRCHandler {
  /**
   * Create and check CRC32 checksums.
   * CRC32 algorithm as described in https://en.wikipedia.org/wiki/Cyclic_redundancy_check#CRC-32_algorithm
   */
 public:
  CRCHandler();
  static crc_value_t calculateCRCValue(const char* data);
  static bool checkCRCValue(const char* data, const crc_value_t checksum);
};

#endif  // CROSSING_CONSOLES_CRCHANDLER_H
