#include "CRCHandler.h"

#include <cstring>

CRCHandler::CRCHandler() = default;

crc_value_t CRCHandler::CalculateCRCValue(const char *data, size_t data_length, bool char_flag) {
  boost::crc_32_type crc_object;
  if (char_flag){
    crc_object.process_bytes(data, data_length);
  }

  return crc_object.checksum();
}

bool CRCHandler::CheckCRCValue(const char *data, size_t data_length, const crc_value_t checksum, bool char_flag) {
  crc_value_t checksum_msg = CalculateCRCValue(data, data_length, char_flag);

  return (checksum_msg == checksum);
}
