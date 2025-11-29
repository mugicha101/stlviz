#include <vector>
#include <iostream>

int main() {
  // INPUT
  std::vector<int> input = { -5, 1, -4, 3, -2 };

  // ALGORITHM
  int output = 0;

  // max positive
  int sum = 0;
  for (int x : input) {
    sum = std::max(sum + x, x);
    output = std::max(output, sum);
  }

  // min negative
  for (int x : input) {
    x = -x;
    sum = std::max(sum + x, x);
    output = std::max(output, sum);
  }

  // OUTPUT
  std::cout << "output: " << output << std::endl;
}
