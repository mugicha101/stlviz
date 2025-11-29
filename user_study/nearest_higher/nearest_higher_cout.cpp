#include <vector>
#include <iostream>
#include <stack>

int main() {
  // INPUT
  std::vector<int> input = { 1, 9, 4, 3, 4, 2, 3, 0 };

  // ALGORITHM
  std::vector<int> output;
  std::stack<int> stack;
  for (int x : input) {
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

  // OUTPUT
  std::cout << "output: [ ";
  for (int x : output) {
    std::cout << x << " ";
  }
  std::cout << "]" << std::endl;
}
