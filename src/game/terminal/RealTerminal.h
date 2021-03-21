#ifndef CROSSING_CONSOLES_REAL_TERMINAL_H
#define CROSSING_CONSOLES_REAL_TERMINAL_H

#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

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

  void SetScreen(const ColoredCharMatrix& content) override;

 private:
  static std::string title;
  /**
   * \brief Set new terminal i/o settings
   * \details Required for `GetCharacter()` and `CharacterWaiting()`.
   * Must be called again if iostream was used (e.g. write to cout).
   */
  static void Initialise();

  /**
   * \brief Clear terminal output.
   */
  void Clear() const;
};

}  // namespace game::terminal

#endif  // CROSSING_CONSOLES_REAL_TERMINAL_H
