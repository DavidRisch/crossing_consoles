#include "CRCHandler.h"

#include <cstring>

using namespace communication;
using namespace communication::message_layer;

CRCHandler::CRCHandler() = default;

crc_value_t CRCHandler::CalculateCRCValue(const uint8_t *data, size_t data_length) {
  boost::crc_32_type crc_object;
  crc_object.process_bytes(data, data_length);
  return crc_object.checksum();
}

bool CRCHandler::CheckCRCValue(const uint8_t *data, size_t data_length, const crc_value_t checksum) {
  crc_value_t checksum_msg = CalculateCRCValue(data, data_length);

  return (checksum_msg == checksum);
}

bool CRCHandler::CalculateAndCheckCRCValue(const crc_value_t checksum) {
  crc_value_t handler_checksum = crc_data.checksum();
  crc_data.reset();
  return (handler_checksum == checksum);
}

void CRCHandler::AppendData(const uint8_t *data, const size_t data_length) {
  crc_data.process_bytes(data, data_length);
}

void CRCHandler::AppendByte(const uint8_t byte) {
  crc_data.process_byte(byte);
}
