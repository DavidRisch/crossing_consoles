#ifndef CROSSING_CONSOLES_DEBUG_H
#define CROSSING_CONSOLES_DEBUG_H

#ifdef CROSSING_CONSOLES_DO_DEBUG

#include <iostream>
#include <mutex>

namespace communication::debug {

extern std::mutex mutex;

}

#define __DEBUG_EXECUTE(statement)                                             \
  do {                                                                         \
    std::lock_guard<std::mutex> debug_lock_guard(communication::debug::mutex); \
    statement;                                                                 \
  } while (false);

#define DEBUG_BYTE_LAYER(statement) __DEBUG_EXECUTE(statement)
#define DEBUG_MESSAGE_LAYER(statement) __DEBUG_EXECUTE(statement)
#define DEBUG_CONNECTION_LAYER(statement) __DEBUG_EXECUTE(statement)
#define DEBUG_TESTS(statement) __DEBUG_EXECUTE(statement)

#else

#define DEBUG_BYTE_LAYER(statement)
#define DEBUG_MESSAGE_LAYER(statement)
#define DEBUG_CONNECTION_LAYER(statement)
#define DEBUG_TESTS(statement)

#endif

#endif  // CROSSING_CONSOLES_DEBUG_H
