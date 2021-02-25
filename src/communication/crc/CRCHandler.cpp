#include "CRCHandler.h"

#include <cstring>

CRCHandler::CRCHandler() = default;

crc_value_t CRCHandler::calculateCRCValue(const char *data) {
  boost::crc_32_type crc_object;
  crc_object.process_bytes(data, strlen(data));

  return crc_object.checksum();
}
bool CRCHandler::checkCRCValue(const char *data, const crc_value_t checksum) {
  crc_value_t checksum_msg = calculateCRCValue(data);

  return (checksum_msg == checksum);
}