#include "MessageMetaData.h"

using namespace communication;
using namespace communication::message_layer;

std::chrono::steady_clock::time_point MessageMetaData::GetTimestampSent() const {
  return timestamp_sent;
}
std::chrono::steady_clock::time_point MessageMetaData::GetTimestampReceived() const {
  return timestamp_received;
}

void MessageMetaData::SetTimestampSent(std::chrono::steady_clock::time_point timestamp) {
  timestamp_sent = timestamp;
}

void MessageMetaData::SetTimestampReceived(std::chrono::steady_clock::time_point timestamp) {
  timestamp_received = timestamp;
}
MessageMetaData::MessageMetaData() = default;
