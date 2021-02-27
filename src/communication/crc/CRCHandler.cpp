#include "CRCHandler.h"

#include <cstring>

CRCHandler::CRCHandler() = default;

crc_value_t CRCHandler::CalculateCRCValue(void const *data, size_t data_length) {
  boost::crc_32_type crc_object;
  crc_object.process_bytes(data, data_length);
  return crc_object.checksum();
}

bool CRCHandler::CheckCRCValue(void const *data, size_t data_length, const crc_value_t checksum) {
  crc_value_t checksum_msg = CalculateCRCValue(data, data_length);

  return (checksum_msg == checksum);
}
