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

  void SetScreen(const std::string &content) override;

  void AddInput(char input);

  [[nodiscard]] const std::string &GetLastOutput() const;

 private:
  std::string last_output;
  std::string unused_inputs;
};

#endif  // CROSSING_CONSOLES_MOCK_TERMINAL_H
