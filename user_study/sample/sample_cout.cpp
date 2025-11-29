#include <vector>
#include <iostream>

int main() {
  // INPUT
  std::vector<int> input = {1,4,3};

  // ALGORITHM
  int output = 0;
  for (int i = 1; i < input.size(); ++i) {
    output += input[i];
  }

  // OUTPUT
  std::cout << "output: " << output << std::endl;
}
