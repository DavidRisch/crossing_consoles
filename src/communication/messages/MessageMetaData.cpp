#include "MessageMetaData.h"

MessageMetaData::MessageMetaData(time_t timestamp_received, time_t timestamp_sent) {
  this->timestamp_sent = timestamp_sent;
  this->timestamp_received = timestamp_received;
}
time_t MessageMetaData::GetTimestampSent() const {
  return timestamp_sent;
}
time_t MessageMetaData::GetTimestampReceived() const {
  return timestamp_received;
}
