#ifndef CROSSING_CONSOLES_DETECT_DEBUGGER_H
#define CROSSING_CONSOLES_DETECT_DEBUGGER_H

#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>

#include <cctype>
#include <cstring>

/// Detect a debugger as described in https://stackoverflow.com/a/24969863
bool runningUnderDebugger();
#endif

#endif  // CROSSING_CONSOLES_DETECT_DEBUGGER_H
