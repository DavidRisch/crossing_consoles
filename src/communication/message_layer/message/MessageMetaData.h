#ifndef CROSSING_CONSOLES_MESSAGEMETADATA_H
#define CROSSING_CONSOLES_MESSAGEMETADATA_H

#include <chrono>
#include <optional>

namespace communication {
namespace message_layer {

/**
 * \brief Meta data about the Message: Timestamps for send and receive
 */
class MessageMetaData {
 public:
  MessageMetaData();
  [[nodiscard]] std::optional<std::chrono::steady_clock::time_point> GetTimestampReceived() const;
  [[nodiscard]] std::optional<std::chrono::steady_clock::time_point> GetTimestampSent() const;
  void SetTimestampReceived(std::chrono::steady_clock::time_point timestamp_received);
  void SetTimestampSent(std::chrono::steady_clock::time_point timestamp);

 private:
  std::optional<std::chrono::steady_clock::time_point> timestamp_received{};
  std::optional<std::chrono::steady_clock::time_point> timestamp_sent{};
};

}  // namespace message_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_MESSAGEMETADATA_H
