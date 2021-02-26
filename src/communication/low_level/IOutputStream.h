#ifndef CROSSING_CONSOLES_I_STREAM_OUT_H
#define CROSSING_CONSOLES_I_STREAM_OUT_H

/**
 * \brief Abstract object with which bytes can be sent.
 */
class IOutputStream {
 public:
  virtual void Send(const uint8_t* send_buffer, size_t length) = 0;
};

#endif  // CROSSING_CONSOLES_I_STREAM_OUT_H
