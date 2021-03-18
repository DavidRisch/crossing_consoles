#include "MockTerminal.h"

using namespace game;
using namespace game::terminal;

bool MockTerminal::HasInput() {
  return !unused_inputs.empty();
}

int MockTerminal::GetInput() {
  int input = (unsigned char)unused_inputs.at(0);
  unused_inputs = unused_inputs.substr(1);
  return input;
}

void MockTerminal::SetScreen(ColoredStringMatrix content) {
  last_output = content;
}

void MockTerminal::AddInput(char input) {
  unused_inputs += input;
}
const ColoredStringMatrix& MockTerminal::GetLastOutput() const {
  return last_output;
}
