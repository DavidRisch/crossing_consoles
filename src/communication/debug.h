#ifndef CROSSING_CONSOLES_DEBUG_H
#define CROSSING_CONSOLES_DEBUG_H

#ifdef CROSSING_CONSOLES_DO_DEBUG

#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

namespace communication {

namespace connection_layer {
class Connection;
}

namespace debug {

extern std::mutex mutex;

/**
 * \brief Used to log messages related to `Connection`s.
 */
class ConnectionLogger {
 public:
  ConnectionLogger();

  /**
   * \brief Prints `log_line` with indentation based on which `Connection` is passed and with the time since the last
   * log entry.
   */
  void Log(connection_layer::Connection* connection, const std::string& log_line);

 private:
  std::vector<connection_layer::Connection*> connections;
  std::chrono::time_point<std::chrono::steady_clock> last_entry_timestamp;
};

extern ConnectionLogger connection_logger;

}  // namespace debug

}  // namespace communication

#define __DEBUG_EXECUTE(statement)                                             \
  do {                                                                         \
    std::lock_guard<std::mutex> debug_lock_guard(communication::debug::mutex); \
    statement;                                                                 \
  } while (false);

#define DEBUG_BYTE_LAYER(statement) __DEBUG_EXECUTE(statement)
#define DEBUG_MESSAGE_LAYER(statement) __DEBUG_EXECUTE(statement)
#define DEBUG_CONNECTION_LAYER(statement) __DEBUG_EXECUTE(statement)
#define DEBUG_TESTS(statement) __DEBUG_EXECUTE(statement)

#define DEBUG_CONNECTION_LOG(connection_ptr, message)                          \
  DEBUG_CONNECTION_LAYER(do {                                                  \
    std::stringstream stream;                                                  \
    stream << message;                                                         \
    communication::debug::connection_logger.Log(connection_ptr, stream.str()); \
  } while (false);)

#else

#define DEBUG_BYTE_LAYER(statement)
#define DEBUG_MESSAGE_LAYER(statement)
#define DEBUG_CONNECTION_LAYER(statement)
#define DEBUG_TESTS(statement)

#define DEBUG_CONNECTION_LOG(ptr, message)

#endif

#endif  // CROSSING_CONSOLES_DEBUG_H
