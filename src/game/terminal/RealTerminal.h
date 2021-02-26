#ifndef CROSSING_CONSOLES_REAL_TERMINAL_H
#define CROSSING_CONSOLES_REAL_TERMINAL_H

#include <string>

#include "ITerminal.h"

/**
 * \brief Uniform interface for the terminal input and output
 */
class RealTerminal : public ITerminal {
 public:
  RealTerminal();

  bool HasInput() override;
  int GetInput() override;

  void SetScreen(const std::string &content) override;

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
  static void Clear();
};

#endif  // CROSSING_CONSOLES_REAL_TERMINAL_H
