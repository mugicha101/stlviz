#include <stlviz.hpp>

int main() {
  // INPUT
  std::vector<int> input = { 92, 89, 74, 100, 61, 70, -63, 70, -75, 75 };
  std::vector<int> queryLeft = { 0, 7, 4 };
  std::vector<int> queryRight = { 2, 9, 4 };

  // ALGORITHM
  std::vector<int> psum(input.size() + 1);
  for (int i = 0; i < input.size(); ++i) {
    psum[i+1] = psum[i] + input[i];
  }
  vstd::vector<int> output;
  DEF(output);
  for (int i = 0; i < queryLeft.size(); ++i) {
    output.push_back(psum[queryRight[i]] - psum[queryLeft[i]]);
  }

  // OUTPUT
  std::cout << "output: [ ";
  for (int x : output) {
    std::cout << x << " ";
  }
  std::cout << "]" << std::endl;
}
