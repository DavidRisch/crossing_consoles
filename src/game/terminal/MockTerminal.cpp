#include "MockTerminal.h"

using namespace game;
using namespace game::terminal;
using namespace game::visual;

bool MockTerminal::HasInput() {
  return !unused_inputs.empty();
}

int MockTerminal::GetInput() {
  int input = (unsigned char)unused_inputs.at(0);
  unused_inputs = unused_inputs.substr(1);
  return input;
}

void MockTerminal::SetScreen(const ColoredCharMatrix& content) {
  last_output = content.GetMatrix();
}

void MockTerminal::AddInput(char input) {
  unused_inputs += input;
}
const std::vector<std::vector<ColoredChar>>& MockTerminal::GetLastOutput() const {
  return last_output;
}
