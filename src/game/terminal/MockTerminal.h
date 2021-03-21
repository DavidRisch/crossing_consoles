#ifndef CROSSING_CONSOLES_MOCK_TERMINAL_H
#define CROSSING_CONSOLES_MOCK_TERMINAL_H

#include <string>

#include "ITerminal.h"

namespace game::terminal {

/**
 * \brief Used for simulating input and capturing output during testing.
 */
class MockTerminal : public ITerminal {
 public:
  bool HasInput() override;
  int GetInput() override;

  void SetScreen(const ColoredCharMatrix& content) override;

  void AddInput(char input);

  [[nodiscard]] const std::vector<std::vector<ColoredChar>>& GetLastOutput() const;

 private:
  std::vector<std::vector<ColoredChar>> last_output;
  std::wstring unused_inputs;
};

}  // namespace game::terminal

#endif  // CROSSING_CONSOLES_MOCK_TERMINAL_H
