#include "CRCHandler.h"

CRCHandler::CRCHandler() = default;

crc_value_t CRCHandler::calculateCRCValue(char *payload) {
  boost::crc_32_type crc_object;
  crc_object.process_bytes(payload, sizeof(&payload) / sizeof(payload[0]));

  return crc_object.checksum();
}
bool CRCHandler::checkCRCValue(char *message, crc_value_t checksum) {
  crc_value_t checksum_msg = calculateCRCValue(message);

  if (checksum_msg == checksum) {
    return true;
  } else {
    return false;
  }
}
