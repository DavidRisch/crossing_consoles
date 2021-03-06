#ifndef CROSSING_CONSOLES_I_TERMINAL_H
#define CROSSING_CONSOLES_I_TERMINAL_H

#include "../visual/ColoredCharMatrix.h"

namespace game::terminal {

/**
 * \brief Uniform interface for the terminal input and output.
 */
class ITerminal {
 public:
  virtual ~ITerminal() = default;

  /**
   * \brief Is an new input available?
   */
  virtual bool HasInput() = 0;

  /**
   * \brief Get new input.
   */
  virtual int GetInput() = 0;

  /**
   * \brief Clear the screen and set it to the specified content.
   */
  virtual void SetScreen(const visual::ColoredCharMatrix& content) = 0;
};

}  // namespace game::terminal

#endif  // CROSSING_CONSOLES_I_TERMINAL_H
