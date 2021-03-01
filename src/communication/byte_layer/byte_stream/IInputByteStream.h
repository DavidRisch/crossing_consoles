#ifndef CROSSING_CONSOLES_I_STREAM_IN_H
#define CROSSING_CONSOLES_I_STREAM_IN_H

namespace communication {
namespace byte_layer {

/**
 * \brief Abstract object with which bytes can be received.
 */
class IInputByteStream {
 public:
  virtual size_t Read(uint8_t* receive_buffer, size_t max_length) = 0;
};

}  // namespace byte_layer
}  // namespace communication

#endif  // CROSSING_CONSOLES_I_STREAM_IN_H
