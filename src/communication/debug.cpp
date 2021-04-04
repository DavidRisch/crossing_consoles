#include "debug.h"

#ifdef CROSSING_CONSOLES_DO_DEBUG

#include <algorithm>
#include <iomanip>

using namespace communication;
using namespace communication::debug;

std::mutex communication::debug::mutex;

communication::debug::ConnectionLogger communication::debug::connection_logger;

void ConnectionLogger::Log(connection_layer::Connection* connection, const std::string& log_line) {
  auto now = std::chrono::steady_clock::now();
  auto delta_time = now - last_entry_timestamp;
  double delta_time_ms = std::chrono::duration_cast<std::chrono::microseconds>(delta_time).count() / 1000.0;
  last_entry_timestamp = now;

  auto it = std::find(connections.begin(), connections.end(), connection);
  int index;
  if (it != connections.end()) {
    index = it - connections.begin();
  } else {
    index = connections.size();
    connections.push_back(connection);
  }
  index %= 4;  // restrict maximum amount of indentation

  int indent_layer_width = 4;  // indent to differentiate between different Connections

  std::string separator = " | ";
  if (delta_time_ms > 1.0) {
    separator = "<@>";  // useful when searching through logs for high delta_time_ms values
  } else if (delta_time_ms > 0.5) {
    separator = " @ ";
  }

  std::cout << std::fixed << std::setprecision(2) << std::setw(7) << delta_time_ms << " ms" << separator
            << std::setw(index * indent_layer_width) << "" << log_line << std::endl;
}

ConnectionLogger::ConnectionLogger()
    : last_entry_timestamp(std::chrono::steady_clock::now()) {
}

#endif
