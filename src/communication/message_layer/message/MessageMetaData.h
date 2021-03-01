#ifndef CROSSING_CONSOLES_MESSAGEMETADATA_H
#define CROSSING_CONSOLES_MESSAGEMETADATA_H

#include <ctime>
class MessageMetaData {
 public:
  MessageMetaData(time_t timestamp_received, time_t timestamp_sent);
  [[nodiscard]] time_t GetTimestampReceived() const;
  [[nodiscard]] time_t GetTimestampSent() const;

 private:
  time_t timestamp_received{};
  time_t timestamp_sent{};
};

#endif  // CROSSING_CONSOLES_MESSAGEMETADATA_H
