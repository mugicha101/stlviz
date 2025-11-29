#include <stlviz.hpp>

int main() {
  // INPUT
  vstd::vector<int> input = { 2, -5, 1, -4, 3, -2 };
  DEF(input);

  // ALGORITHM
  vstd::Int output = 0;
  DEF(output);

  // max positive
  vstd::Int sum = 0;
  DEF(sum);
  for (vstd::Int x : input) {
    DEF(x);
    sum = std::max(sum + x, (int)x);
    output = std::max(output, sum);
  }

  // min negative
  for (vstd::Int x : input) {
    DEF(x);
    x = -x;
    sum = std::max(sum + x, (int)x);
    output = std::max(output, sum);
  }

  // OUTPUT - check output
  MAIN_DONE;
}
