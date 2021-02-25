#include "CRCHandler.h"

#include <iostream>

CRCHandler::CRCHandler() = default;

crc_value_t CRCHandler::calculateCRCValue(const char *payload) {
  boost::crc_32_type crc_object ;

  crc_object.process_bytes(&payload[0], sizeof(payload));

  return crc_object.checksum();
}
bool CRCHandler::checkCRCValue(char *message, crc_value_t checksum) {
  crc_value_t checksum_msg = calculateCRCValue(message);

  return (checksum_msg == checksum);

}