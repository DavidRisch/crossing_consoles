#ifndef CROSSING_CONSOLES_REAL_TERMINAL_H
#define CROSSING_CONSOLES_REAL_TERMINAL_H

#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "ITerminal.h"

/**
 * \brief Uniform interface for the terminal input and output
 */
class RealTerminal : public ITerminal {
 public:
  static std::string title;
#ifdef _WIN32
  static HANDLE console;
#endif
  RealTerminal();

  bool HasInput() override;
  int GetInput() override;

  void SetScreen(const std::wstring &content) override;

 private:
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

#endif  // CROSSING_CONSOLES_REAL_TERMINAL_H
