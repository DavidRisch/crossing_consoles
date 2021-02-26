#ifndef CROSSING_CONSOLES_TERMINAL_H
#define CROSSING_CONSOLES_TERMINAL_H

#ifndef _WIN32
#include <termios.h>
#endif

/**
 * \brief Uniform interface for the terminal (incl. keyboard) for Linux and Windows
 */
class Terminal {
 public:
  /**
   * \brief Set new terminal i/o settings
   * \details Required for `GetCharacter()` and `CharacterWaiting()`.
   * Must be called again if iostream was used (e.g. write to cout).
   */
  static void Initialise();

  /**
   * \brief Read 1 character.
   */
  static int GetCharacter();

  /**
   * \brief Check if a new character can be read now.
   */
  static bool CharacterWaiting();

  /**
   * \brief Clear terminal output.
   */
  static void Clear();
};

#endif  // CROSSING_CONSOLES_TERMINAL_H
