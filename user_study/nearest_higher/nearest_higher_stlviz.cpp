#include <stlviz.hpp>

int main() {
  // INPUT
  vstd::vector<int> input = { 1, 9, 4, 3, 4, 2, 3, 0 };
  DEF(input);

  // ALGORITHM
  vstd::vector<int> output;
  DEF(output);
  vstd::stack<int> stack;
  DEF(stack);
  for (vstd::Int x : input) {
    DEF(x);
    while (!stack.empty() && stack.top() < x) {
      stack.pop();
    }
    if (stack.empty()) {
      output.push_back(-1);
    } else {
      output.push_back(stack.top());
    }
    stack.push(x);
  }

  // OUTPUT - check output
  MAIN_DONE;
}
