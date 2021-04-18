#ifndef CROSSING_CONSOLES_REAL_TERMINAL_H
#define CROSSING_CONSOLES_REAL_TERMINAL_H

#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

#include "../common/Color.h"
#include "../visual/ColoredString.h"
#include "ITerminal.h"

namespace game::terminal {

/**
 * \brief Uniform interface for the terminal input and output
 */
class RealTerminal : public ITerminal {
 public:
  RealTerminal();

  bool HasInput() override;
  int GetInput() override;

  void SetScreen(const visual::ColoredCharMatrix& content) override;

 private:
  static std::string title;
  /**
   * \brief Set new terminal i/o settings
   * \details Required for `GetCharacter()` and `CharacterWaiting()`.
   * Must be called again if iostream was used (e.g. write to cout).
   */
  static void Initialise();

  /**
   * \brief Generate an escape sequence to set the specified color as the fore- or background color.
   */
  inline std::string ColorEscapeSequence(const common::Color& color, bool background);

  /**
   * \brief Clear terminal output.
   */
  void Clear();

  /**
   * \brief Set `redraw_needed` to true if a call to `Clear()` is required to fix artifacts in the terminal.
   */
  void CheckTerminalChanged();

  bool redraw_needed = true;

  visual::ColoredCharMatrix last_screen_content{{0, 0}};

#ifndef _WIN32
  struct winsize terminal_size {};
#endif
};

}  // namespace game::terminal

#endif  // CROSSING_CONSOLES_REAL_TERMINAL_H
