#include "RealTerminal.h"

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#include <fcntl.h>

#else

#include <sys/ioctl.h>
#include <termios.h>

#include <codecvt>
#include <locale>
#endif

std::string RealTerminal::title = "Crossing Consoles";

#ifdef _WIN32
HANDLE RealTerminal::console = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

RealTerminal::RealTerminal() {
  Initialise();
}

bool RealTerminal::HasInput() {
#ifdef _WIN32
  return _kbhit();
#else
  int bytes_waiting;
  ioctl(0, FIONREAD, &bytes_waiting);
  return bytes_waiting > 0;
#endif
}

int RealTerminal::GetInput() {
#ifdef _WIN32
  return _getch();
#else
  return getchar();
#endif
}

void RealTerminal::SetScreen(const std::wstring& content) {
  Clear();
#ifdef _WIN32
  _cwprintf(content.c_str());
#else
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::string narrow = converter.to_bytes(content);
  printf("%s", narrow.c_str());
#endif
}

void RealTerminal::Initialise() {
#ifdef _WIN32
  SetConsoleTitle(title.c_str());
  _setmode(_fileno(stdout), _O_U16TEXT);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = FALSE;
  SetConsoleCursorInfo(console, &info);
#else
  // https://stackoverflow.com/a/7469410/13623303
  struct termios current {};
  tcgetattr(0, &current);           // grab old terminal i/o settings
  current.c_lflag &= ~ICANON;       // disable buffered i/o
  current.c_lflag &= ~ECHO;         // set no echo mode
  tcsetattr(0, TCSANOW, &current);  // use these new terminal i/o settings now
#endif
}

void RealTerminal::Clear() const {
#ifdef _WIN32
  SetConsoleCursorPosition(console, {0, 0});
#else
  system("clear");
#endif
}
