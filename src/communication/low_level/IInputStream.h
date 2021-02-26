#ifndef CROSSING_CONSOLES_I_STREAM_IN_H
#define CROSSING_CONSOLES_I_STREAM_IN_H

/**
 * \brief Abstract object with which bytes can be received.
 */
class IInputStream {
 public:
  virtual size_t Read(uint8_t* receive_buffer, size_t max_length) = 0;
};

#endif  // CROSSING_CONSOLES_I_STREAM_IN_H
