#ifndef CROSSING_CONSOLES_I_STREAM_OUT_H
#define CROSSING_CONSOLES_I_STREAM_OUT_H

namespace communication {
namespace byte_layer {

/**
 * \brief Abstract object with which bytes can be sent.
 */
class IOutputByteStream {
 public:
  virtual ~IOutputByteStream() = default;

  virtual void Send(const uint8_t* send_buffer, size_t length) = 0;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_I_STREAM_OUT_H
