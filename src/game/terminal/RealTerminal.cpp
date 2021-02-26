#include "RealTerminal.h"

#include <cstdlib>

#ifdef _WIN32
#include <conio.h>
#else

#include <sys/ioctl.h>
#include <termios.h>

#include <cstdio>
#endif

RealTerminal::RealTerminal() {
  Initialise();
}

bool RealTerminal::HasInput() {
#ifdef _WIN32
  return kbhit();
#else
  int bytes_waiting;
  ioctl(0, FIONREAD, &bytes_waiting);
  return bytes_waiting > 0;
#endif
}

int RealTerminal::GetInput() {
#ifdef _WIN32
  return getch();
#else
  return getchar();
#endif
}

void RealTerminal::SetScreen(const std::string& content) {
  printf("%s", content.c_str());
}

void RealTerminal::Initialise() {
#ifdef _WIN32
  // no configuration required
#else
  // https://stackoverflow.com/a/7469410/13623303
  struct termios current {};
  tcgetattr(0, &current);           // grab old terminal i/o settings
  current.c_lflag &= ~ICANON;       // disable buffered i/o
  current.c_lflag &= ~ECHO;         // set no echo mode
  tcsetattr(0, TCSANOW, &current);  // use these new terminal i/o settings now
#endif
}

void RealTerminal::Clear() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}
