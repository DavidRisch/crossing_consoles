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

using namespace game;
using namespace game::terminal;

std::string RealTerminal::title = "Crossing Consoles";

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

void RealTerminal::SetScreen(ColoredCharMatrix content) {
  Clear();
#ifdef _WIN32
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

  ColoredString colored_string = content.GetString();
  while (colored_string.string != std::wstring(1, L'\0')) {
    SetConsoleTextAttribute(console_handle, (colored_string.background << 4) | colored_string.foreground);
    _cwprintf(colored_string.string.c_str());
    colored_string = content.GetString();
  }
#else
  ColoredString colored_string = content.GetString();
  while (colored_string.string != std::wstring(1, L'\0')) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string narrow = converter.to_bytes(colored_string.string);
    printf("\033[%d;%dm", colored_string.foreground, colored_string.background + background_color_offset);
    printf("%s", narrow.c_str());
    printf("\033[0m");
    colored_string = content.GetString();
  }
#endif
}

void RealTerminal::Initialise() {
#ifdef _WIN32
  _setmode(_fileno(stdout), _O_U16TEXT);
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

  SetConsoleTitle(title.c_str());

  // contains information about the console cursor
  CONSOLE_CURSOR_INFO cursor_info;
  // visibility of the cursor
  cursor_info.bVisible = FALSE;
  SetConsoleCursorInfo(console_handle, &cursor_info);

  CONSOLE_FONT_INFOEX font_info;
  font_info.cbSize = sizeof font_info;
  font_info.nFont = 0;
  font_info.dwFontSize.X = 8;
  font_info.dwFontSize.Y = 8;
  font_info.FontFamily = FF_DONTCARE;
  font_info.FontWeight = FW_NORMAL;
  wcscpy(font_info.FaceName, L"Terminal");
  SetCurrentConsoleFontEx(console_handle, FALSE, &font_info);
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
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(console_handle, {0, 0});
#else
  system("clear");
#endif
}
