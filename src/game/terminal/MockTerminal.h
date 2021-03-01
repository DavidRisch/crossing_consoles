#ifndef CROSSING_CONSOLES_MOCK_TERMINAL_H
#define CROSSING_CONSOLES_MOCK_TERMINAL_H

#include <string>

#include "ITerminal.h"

/**
 * \brief Used for simulating input and capturing output during testing.
 */
class MockTerminal : public ITerminal {
 public:
  bool HasInput() override;
  int GetInput() override;

  void SetScreen(const std::wstring &content) override;

  void AddInput(char input);

  [[nodiscard]] const std::wstring &GetLastOutput() const;

 private:
  std::wstring last_output;
  std::wstring unused_inputs;
};

#endif  // CROSSING_CONSOLES_MOCK_TERMINAL_H
