#include "RealTerminal.h"

#include "../visual/ColoredString.h"

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
using namespace game::visual;

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

void RealTerminal::SetScreen(const ColoredCharMatrix& content) {
  Clear();

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

  const std::vector<std::vector<ColoredChar>>& colored_characters = content.GetMatrix();

  ColoredString colored_string(std::wstring(), colored_characters[0][0].foreground,
                               colored_characters[0][0].background);
#ifdef _WIN32
  std::wstring output;
#else
  std::string output;
#endif

  for (const auto& i_lines : colored_characters) {
    for (const auto& i_characters : i_lines) {
      // check if character colors are equal to string colors
      if (i_characters.foreground == colored_string.foreground &&
          i_characters.background == colored_string.background) {
        // append current character
        colored_string.string.push_back(i_characters.character);
      } else {
        // add current string to output, print later
#ifdef _WIN32
        output += converter.from_bytes(ColorEscapeSequence(colored_string.foreground, false));
        output += converter.from_bytes(ColorEscapeSequence(colored_string.background, true));
        output += colored_string.string;
#else
        output += ColorEscapeSequence(colored_string.foreground, false);
        output += ColorEscapeSequence(colored_string.background, true);
        output += converter.to_bytes(colored_string.string);
#endif

        // reset string
        colored_string.string = std::wstring(1, i_characters.character);
        colored_string.foreground = i_characters.foreground;
        colored_string.background = i_characters.background;
      }
    }
    colored_string.string.push_back(L'\n');
  }
  // print last string
#ifdef _WIN32
  output += converter.from_bytes(ColorEscapeSequence(colored_string.foreground, false));
  output += converter.from_bytes(ColorEscapeSequence(colored_string.background, true));
  output += colored_string.string;
#else
  output += ColorEscapeSequence(colored_string.foreground, false);
  output += ColorEscapeSequence(colored_string.background, true);
  output += converter.to_bytes(colored_string.string);
#endif

  output += "\033[0m \n";  // reset colors to prevent flickering of background

  output = "\033[1;1H" + output;  // reset cursor to the top left (dont clear to prevent flickering)

#ifdef _WIN32
  _cwprintf(output.c_str());
#else
  printf("%s", output.c_str());
#endif
}

void RealTerminal::Initialise() {
#ifdef _WIN32
  _setmode(_fileno(stdout), _O_U16TEXT);
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

  SetConsoleTitle(title.c_str());

  // https://docs.microsoft.com/en-us/windows/console/console-cursor-info-str
  CONSOLE_CURSOR_INFO cursor_info;
  cursor_info.bVisible = FALSE;
  SetConsoleCursorInfo(console_handle, &cursor_info);

  // https://docs.microsoft.com/en-us/windows/console/console-font-infoex
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
#endif
  // On Linux the screen is overwritten using ANSI escape codes
}

std::string RealTerminal::ColorEscapeSequence(const common::Color& color, bool background) {
  // 24 bit color ANSI escape code, see https://en.wikipedia.org/wiki/ANSI_escape_code#24-bit
  std::string output;
  output += "\033[";
  if (background) {
    output += "48";
  } else {
    output += "38";
  }
  output += ";2;";
  output += std::to_string(color.red);
  output += ";";
  output += std::to_string(color.green);
  output += ";";
  output += std::to_string(color.blue);
  output += "m";
  return output;
}
