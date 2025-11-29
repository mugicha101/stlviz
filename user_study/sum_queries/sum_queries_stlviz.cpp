#include <stlviz.hpp>

int main() {
  // INPUT
  vstd::vector<int> input = { 92, 89, 74, 100, 61, 70, -63, 70, -75, 75 };
  DEF(input);
  vstd::vector<int> queryLeft = { 0, 7, 4 };
  DEF(queryLeft);
  vstd::vector<int> queryRight = { 2, 9, 4 };
  DEF(queryRight);

  // ALGORITHM
  vstd::vector<int> psum(input.size() + 1);
  DEF(psum);
  for (int i = 0; i < input.size(); ++i) {
    psum[i+1] = psum[i] + input[i];
  }
  vstd::vector<int> output;
  DEF(output);
  for (int i = 0; i < queryLeft.size(); ++i) {
    output.push_back(psum[queryRight[i]] - psum[queryLeft[i]]);
  }

  // OUTPUT - check output
  MAIN_DONE;
}
